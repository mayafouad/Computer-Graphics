
#include <iostream>
#include <Windows.h>
#include <algorithm>
#include <cmath>
using namespace std;

void DrawLineBresenham(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
    // We must ensure that dx >= 0 inorder to have 4 cases otherwise we will have 8 cases 
    if (x1 > x2) {
        swap(x1, x2);
        swap(y1, y2);
    }
    int dx = x2 - x1;
    int dy = y2 - y1;
    double slope = (double)dy / dx;

    if (dy >= 0) {
        if (slope < 1) { // CASE [1]: dy >= 0 && slope < 1
            int d = dx - 2 * dy;
            int d1 = 2 * (dx - dy);
            int d2 = -2 * dy;
            int x = x1, y = y1;
            SetPixel(hdc, x, y, c);

            while (x < x2) {
                if (d <= 0) {
                    x++; y++;
                    d += d1;
                }
                else {
                    x++;
                    d += d2;
                }
                SetPixel(hdc, x, y, c);
            }

        }
        else { // CASE [2]: dy >= 0 && slope >= 1
            int d = 2 * dx - dy;
            int d1 = 2 * (dx - dy);
            int d2 = 2 * dx;
            int x = x1, y = y1;
            SetPixel(hdc, x, y, c);

            while (y < y2) {
                if (d > 0) {
                    x++; y++;
                    d += d1;
                }
                else {
                    y++;
                    d += d2;
                }
                SetPixel(hdc, x, y, c);
            }

        }
    }
    else {
        if (abs(slope) < 1) { // CASE [3]: dy < 0 && slope < 1
            int d = -dx - 2 * dy;
            int d1 = -2 * (dx + dy);
            int d2 = -2 * dy;
            int x = x1, y = y1;
            SetPixel(hdc, x, y, c);

            while (x < x2) {
                if (d > 0) {
                    x++; y--;
                    d += d1;
                }
                else {
                    x++;
                    d += d2;
                }
                SetPixel(hdc, x, y, c);
            }

        }
        else { // CASE [4]: dy < 0 && slope >= 1
            int d = -2 * dx - dy;
            int d1 = -2 * (dx + dy);
            int d2 = -2 * dx;
            int x = x1, y = y1;
            SetPixel(hdc, x, y, c);

            while (y > y2) {
                if (d <= 0) {
                    x++; y--;
                    d += d1;
                }
                else {
                    y--;
                    d += d2;
                }
                SetPixel(hdc, x, y, c);
            }

        }


    }
}
void DrawSimilarPoints(HDC hdc, int x_center, int y_center, int x, int y, COLORREF c) {

    SetPixel(hdc, x_center + x, y_center + y, c);
    SetPixel(hdc, x_center - x, y_center + y, c);
    SetPixel(hdc, x_center + x, y_center - y, c);
    SetPixel(hdc, x_center - x, y_center - y, c);

    SetPixel(hdc, x_center + y, y_center + x, c);
    SetPixel(hdc, x_center - y, y_center + x, c);
    SetPixel(hdc, x_center + y, y_center - x, c);
    SetPixel(hdc, x_center - y, y_center - x, c);
}


void DrawCircle(HDC hdc, int x_center, int y_center, int radius, COLORREF c) {
    int x = 0, y = radius;
    int  d = 1 - radius;

    while (x < y) {
        DrawSimilarPoints(hdc, x_center, y_center, x, y, c);
        if (d < 0) { // Inside the circle
            d += 2 * x + 3;
            x++;
        }
        else {
            d += 2 * (x - y) + 5;
            x++; y--;
        }
    }
}


void DrawCircleBresenhamseDoubleDDA(HDC hdc, int x_center, int y_center, int radius, COLORREF c, bool fill) {
    int x = 0, y = radius;
    int d = 1 - radius;
    int d1Initial = 3;
    int d2Initial = 5 - 2 * radius;
    DrawSimilarPoints(hdc, x_center, y_center, x, y, c);
    while (x < y) {
        if (fill) {
            DrawLineBresenham(hdc, x_center, y_center, x_center + x, y_center + y, c);
            DrawLineBresenham(hdc, x_center, y_center, x_center - x, y_center - y, c);

        }
        if (d < 0) {
            d += d1Initial;
            d1Initial += 2;
            d2Initial += 2;
            x++;
        }
        else {
            d += d2Initial;
            d1Initial += 2;
            d2Initial += 4;
            x++;
            y--;
        }
        DrawSimilarPoints(hdc, x_center, y_center, x, y, c);
    }
}
int x_1,  y_1;
int x_2,  y_2;
int x_3,  y_3;
int leftPointsCnt = 0;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    HDC hdc;


    switch (uMsg) {
    case WM_LBUTTONDOWN: {
        if (leftPointsCnt == 0) {
            x_1 = LOWORD(lParam);
            y_1 = HIWORD(lParam);
            leftPointsCnt++;
        }
        else {
            x_2 = LOWORD(lParam);
            y_2 = HIWORD(lParam);
        }
        break;
    }
    
    case WM_RBUTTONDOWN: {
        x_3 = LOWORD(lParam);
        y_3 = HIWORD(lParam);
        HDC hdc = GetDC(hwnd);
        int r1 = (int)sqrt(pow(x_1 - x_3, 2) + pow(y_1 - y_3, 2));
        int r2 = (int)sqrt(pow(x_2 - x_3, 2) + pow(y_2 - y_3, 2));

        DrawCircleBresenhamseDoubleDDA(hdc, x_1, y_1, r1, RGB(255, 0, 255), r1 >= r2);
        DrawCircleBresenhamseDoubleDDA(hdc, x_2, y_2, r2, RGB(255, 0, 255), r1 < r2);
        
        ReleaseDC(hwnd, hdc);
        break;
    }

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}


int APIENTRY WinMain(HINSTANCE hi, HINSTANCE pi, LPSTR cmd, int nsh)
{
    WNDCLASS wc = { 0 };
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.lpszClassName = L"MyClass";
    wc.lpszMenuName = NULL;
    wc.lpfnWndProc = WindowProc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hInstance = hi;

    RegisterClass(&wc);
    HWND hwnd = CreateWindow(L"MyClass", L"Line Drawing", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, NULL, NULL, hi, NULL);

    ShowWindow(hwnd, nsh);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}
