#include "CPPLibrary.h"
#include "framework.h"
#include "WindowsProject1.h"
#include "Windows.h"
#define MAX_LOADSTRING 100
using namespace std;

struct Point {
    int x, y;
    Point(int d, int z) : x(d), y(z) {}
};

int Round(double y) {
    return (int)(y + 0.5);
}

void DrawLineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
    SetPixel(hdc, x1, y1, color);
    int dx = x2 - x1, dy = y2 - y1;
    if (abs(dx) >= abs(dy)) {
        if (dx == 0)return;
        double m = (double)dy / dx;
        double y = y1;
        int x = x1;
        while (x < x2) {
            x++;
            y += m;
            SetPixel(hdc, x, Round(y), color);
        }
    }
    else {
        if (dy == 0)return;
        double m = (double)dx / dy;
        double x = x1;
        int y = y1;
        while (y < y2) {
            y++;
            x += m;
            SetPixel(hdc, Round(x), y, color);
        }
    }
}

void DrawEightPoints(HDC hdc, int XCeneter, int YCenter, int X, int Y, COLORREF color) {
    SetPixel(hdc, XCeneter + X, YCenter + Y, color);
    SetPixel(hdc, XCeneter + X, YCenter - Y, color);
    SetPixel(hdc, XCeneter - X, YCenter + Y, color);
    SetPixel(hdc, XCeneter - X, YCenter - Y, color);
    SetPixel(hdc, XCeneter + Y, YCenter + X, color);
    SetPixel(hdc, XCeneter + Y, YCenter - X, color);
    SetPixel(hdc, XCeneter - Y, YCenter + X, color);
    SetPixel(hdc, XCeneter - Y, YCenter - X, color);
}
int cnt = 0;
void DrawCircleBresenham(HDC hdc, int XCenter, int YCenter, int Radius, COLORREF color) {
    cnt++;
    int x = 0, y = Radius;
    DrawEightPoints(hdc, XCenter, YCenter, x, y, color);
    int d = 1 - Radius, delta1 = 3, delta2 = 5 - (2 * Radius);
    while (x < y) {
        if (d >= 0) {
            d += delta2;
            delta2 += 4;
            y--;

        }
        else {
            d += delta1;
            delta2 += 2;
        }
        delta1 += 2;
        x++;
        DrawEightPoints(hdc, XCenter, YCenter, x, y, color);
        if(cnt == 1){
            DrawLineDDA(hdc, XCenter, YCenter, x + XCenter, y + YCenter, color);
            DrawLineDDA(hdc, XCenter, YCenter, XCenter + y, YCenter + x, color);
        }
    }

}



/* Function responsible for taking Action related to Mouse */
vector <Point> Points;
LRESULT static WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp)
{
    HDC hdc ;
    int x = 0, y = 0 , Radius;
    switch (m) {
    case WM_LBUTTONDOWN:
        x = LOWORD(lp);
        y = HIWORD(lp);
        Points.push_back(Point(x , y));
        break;
    case WM_RBUTTONDOWN:
        hdc = GetDC(hwnd);
        x = LOWORD(lp);
        y = HIWORD(lp);
        Radius = (int)sqrt(pow(Points[0].x - x, 2) + pow(Points[0].y - y , 2));
        cnt = 0;
        DrawCircleBresenham(hdc , Points[0].x , Points[0].y , Radius , RGB(190,180,120));
        Radius = (int)sqrt(pow(Points[1].x - x, 2) + pow(Points[1].y - y, 2));
        DrawCircleBresenham(hdc, Points[1].x, Points[1].y, Radius, RGB(120, 120, 120));
        Points.clear();
        ReleaseDC(hwnd, hdc);
        break;
    case WM_LBUTTONUP:
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd); break;
    case WM_DESTROY:
        PostQuitMessage(0); break;
    default:return DefWindowProc(hwnd, m, wp, lp);
    }
    return 0;
}

/* Main Entry Point Function */

int APIENTRY WinMain(HINSTANCE hi, HINSTANCE pi, LPSTR cmd, int nsh)
{
    WNDCLASS wc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH) ; 
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); 
    wc.lpszClassName = L"MyClass";   
    wc.lpszMenuName = NULL; 
    wc.lpfnWndProc = WndProc;
    wc.style = CS_HREDRAW | CS_VREDRAW; 
    wc.hInstance = hi;
    RegisterClass(&wc);
    HWND hwnd = CreateWindow(L"MyClass", L"Hello World!", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hi, 0);
    ShowWindow(hwnd, nsh);
    UpdateWindow(hwnd);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

