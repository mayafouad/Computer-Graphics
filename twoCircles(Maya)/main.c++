#include <windows.h>
#include <cmath>
#include <vector>
using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawParametricLine(HDC hdc, int x1, int y_start, int x2, int y_end);
void CircleFasterBresenham(HDC hdc, int xc, int yc, int R, COLORREF color, bool f);
void Draw8Points(HDC hdc, int xc, int yc, int a, int b, COLORREF color, bool f);

int c1_x, c1_y, c2_x, c2_y, p_x, p_y;
int c = 0;
COLORREF color1 = RGB(90, 50, 90);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "Draw2Circles";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);
    HWND hwnd = CreateWindow(wc.lpszClassName, "Draw2Circles", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HDC hdc;

    switch (msg) {
    case WM_LBUTTONDOWN: {
        if(c%2 == 0){
            c1_x = LOWORD(lParam);
            c1_y = HIWORD(lParam);
        }
        else{
            c2_x = LOWORD(lParam);
            c2_y = HIWORD(lParam);
        }
        c++;
        break;
    }


    case WM_RBUTTONDOWN: {
        p_x = LOWORD(lParam);
        p_y = HIWORD(lParam);
        hdc = GetDC(hwnd);
        // Circle 1
        int dx1 = abs(p_x - c1_x), dy1 = abs(p_y - c1_y);
        int r1 = int(sqrt(dx1 * dx1 + dy1 * dy1));
        CircleFasterBresenham(hdc, c1_x, c1_y, r1, color1, true);

        // Circle 2
        int dx2 = abs(p_x - c2_x), dy2 = abs(p_y - c2_y);
        int r2 = int(sqrt(dx2 * dx2 + dy2 * dy2));
        CircleFasterBresenham(hdc, c2_x, c2_y, r2, color1, false);

        ReleaseDC(hwnd, hdc);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}


void DrawParametricLine(HDC hdc, int x_start, int y_start, int x_end, int y_end) {
    int alpha1 = x_end - x_start;
    int alpha2 = y_end - y_start;
    double step = 1.0 / max(abs(alpha1), abs(alpha2));
    for (double t = 0; t <= 1; t += step) {
        int x = round(alpha1 * t + x_start);
        int y = round(alpha2 * t + y_start);
        SetPixel(hdc, x, y, color1);
    }
}


void CircleFasterBresenham(HDC hdc, int xc, int yc, int R, COLORREF color, bool f) {
    int x = 0, y = R;
    int d = 1 - R;
    int c1 = 3, c2 = 5 - 2 * R;
    Draw8Points(hdc, xc, yc, x, y, color, f);
    while (x < y) {
        if (d < 0) {
            d += c1;
            c2 += 2;
        }
        else {
            d += c2;
            c2 += 4;
            y--;
        }
        c1 += 2;
        x++;
        Draw8Points(hdc, xc, yc, x, y, color, f);
    }
}

void Draw8Points(HDC hdc, int xc, int yc, int a, int b, COLORREF color, bool f) {
    SetPixel(hdc, xc + a, yc + b, color);
    if(f) DrawParametricLine(hdc, xc, yc, xc + a, yc + b);
    SetPixel(hdc, xc - a, yc + b, color);
    if(f) DrawParametricLine(hdc, xc, yc, xc - a, yc + b);
    SetPixel(hdc, xc - a, yc - b, color);
    SetPixel(hdc, xc + a, yc - b, color);
    SetPixel(hdc, xc + b, yc + a, color);
    SetPixel(hdc, xc - b, yc + a, color);
    SetPixel(hdc, xc - b, yc - a, color);
    SetPixel(hdc, xc + b, yc - a, color);
}
