#include <Windows.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <cmath>
using namespace std;


struct Point {

    int x, y;
    Point() : Point(0, 0) {}
    Point(int x, int y) : x(x), y(y) {}
};

Point points[5];  // Store five points
int pointCount = 0; // Keep track of how many points have been clicked




void DrawLineBresenham(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {

    if (x1 > x2) {
        swap(x1, x2);
        swap(y1, y2);
    }
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);


    if (dx >= dy) {
        int x = x1, y = y1;

        SetPixel(hdc, x, y, color);

        int d = dx - (2 * dy);
        int d1 = -2 * dy;
        int d2 = 2 * (dx - dy);
        int incY = 1;
        if (y2 < y1) {
            incY = -1;
        }
        while (x < x2) {
            if (d > 0) {
                x++;
                d += d1;
            }
            else {
                x++;
                y += incY;
                d += d2;
            }
            SetPixel(hdc, x, y, color);
        }
    }
    else {
        if (y1 > y2) {
            swap(y1, y2);
            swap(x1, x2);
        }
        int x = x1, y = y1;

        SetPixel(hdc, x, y, color);
        int d = dx - (2 * dy);
        int d1 = 2 * dx;
        int d2 = 2 * (dx - dy);
        int incX = 1;
        if (x2 < x1) {
            incX = -1;
        }
        while (y < y2) {
            if (d < 0) {
                y++;
                d += d1;
            }
            else {
                y++;
                x += incX;
                d += d2;
            }
            SetPixel(hdc, x, y, color);
        }
    }
}

Point GetMidPoint(Point p1, Point p2) {
    return Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
}

COLORREF GetRandColor() {

    double r = static_cast<double>(std::rand()) / RAND_MAX;
    double g = static_cast<double>(std::rand()) / RAND_MAX;
    double b = static_cast<double>(std::rand()) / RAND_MAX;
    return RGB(255 * r, 255 * g, 255 * b);

}
// Compute center of five points
Point computeCenter(Point points[]) {
    Point center = { 0, 0 };
    for (int i = 0; i < 5; i++) {
        center.x += points[i].x;
        center.y += points[i].y;
    }
    center.x /= 5;
    center.y /= 5;
    return center;
}

void DrawStarPart(HDC hdc, Point p1, Point p2, Point center) {
    Point mid = GetMidPoint(p1, p2);
    Point other = GetMidPoint(mid, center);
    DrawLineBresenham(hdc, p1.x, p1.y, other.x, other.y, GetRandColor());
    DrawLineBresenham(hdc, p2.x, p2.y, other.x, other.y, GetRandColor());
}

void StarDrawingHollow(HDC hdc, Point points[]) {

    for (int i = 0; i < 5; i++) {
        DrawStarPart(hdc, points[i], points[(i + 1) % 5], computeCenter(points));
    }

}

void StarDrawingLines(HDC hdc, Point points[]) {

    int starOrder[] = { 0, 2, 4, 1, 3, 0 }; // Correct star order

    for (int i = 0; i < 5; i++) {
        DrawLineBresenham(hdc, points[starOrder[i]].x, points[starOrder[i]].y,
            points[starOrder[i + 1]].x, points[starOrder[i + 1]].y, GetRandColor());
    }


}




// Sort points counterclockwise around the center
bool angleCompare(Point a, Point b, Point center) {
    double angleA = atan2(a.y - center.y, a.x - center.x);
    double angleB = atan2(b.y - center.y, b.x - center.x);
    return angleA < angleB;
}




void (*DrawStar) (HDC hdc, Point points[]) = StarDrawingHollow;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    HDC hdc;


    switch (uMsg) {
    case WM_LBUTTONDOWN: {
        if (pointCount < 5) {
            points[pointCount].x = LOWORD(lParam);
            points[pointCount].y = HIWORD(lParam);
            pointCount++;

            if (pointCount == 5) {
                hdc = GetDC(hwnd);


                Point center = computeCenter(points);

                // Sort points counterclockwise
                sort(points, points + 5, [center](Point a, Point b) {
                    return angleCompare(a, b, center);
                    });

                DrawStar(hdc, points);


                ReleaseDC(hwnd, hdc);
                pointCount = 0;
            }
        }
        break;
    }

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}





int APIENTRY WinMain(HINSTANCE hi, HINSTANCE pi, LPSTR cmd, int nsh)
{
 //   std::srand(std::time(0));

    WNDCLASS wc = { 0 };
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.lpszClassName = L"StarDrawing";
    wc.lpszMenuName = NULL;
    wc.lpfnWndProc = WindowProc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hInstance = hi;

    RegisterClass(&wc);
    HWND hwnd = CreateWindow(L"StarDrawing", L"Star Drawing", WS_OVERLAPPEDWINDOW,
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
