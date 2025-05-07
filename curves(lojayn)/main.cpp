
#include <iostream>
#include <Windows.h>
#include <algorithm>
#include <cmath>
#include <vector>
using namespace std;


vector<vector<int>> Hermite =  {
    {2, 1, -2, 1},
    {-3, -2, 3, -1},
    {0, 1, 0, 0},
    {1, 0, 0, 0}
};

vector<vector<int>> Bezier = {
    {-1,  3, -3,  1},  
    { 3, -6,  3,  0},  
    {-3,  3,  0,  0},  
    { 1,  0,  0,  0}   
};

struct Point {
    int x, y;
    Point():Point(0, 0) {}
    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }

    friend Point operator*(double t, const Point& p) {
        return Point(t * p.x, t * p.y);
    }
    friend Point operator+(const Point& p1,  const Point& p) {
        return Point(p.x + p1.x, p.y + p1.y);
    }
    friend Point operator-(const Point& p1, const Point& p) {
        return Point(p1.x - p.x, p1.y - p.y);
    }
};

vector<vector<int>> MultiplyMatrix(vector<vector<int>> Mat1, vector<vector<int>> Mat2) {
    int rowsMat1 = Mat1.size();
    int colsMat1 = Mat1[0].size();
    int rowsMat2 = Mat2.size();
    int colsMat2 = Mat2[0].size();

   
    vector<vector<int>> result(rowsMat1, vector<int>(colsMat2, 0));


    for (int i = 0; i < rowsMat1; i++) {
        for (int j = 0; j < colsMat2; j++) {
            for (int k = 0; k < colsMat1; k++) {
                result[i][j] += Mat1[i][k] * Mat2[k][j];
            }
        }
    }

    return result;
}

void DrawHermite(HDC hdc, Point p1, Point s1, Point p2, Point s2, COLORREF c) {
    vector<vector<int>> Points = {
        {p1.x, p1.y},
        {s1.x, s1.y},
        {p2.x, p2.y},
        {s2.x, s2.y}
    };
    vector<vector<int>> coff = MultiplyMatrix(Hermite, Points);
    double x, y;
    for (double t = 0; t <= 1.0; t += 0.001) {
        
        x = coff[0][0] * pow(t, 3) + coff[1][0] * pow(t, 2) + coff[2][0] * t + coff[3][0];
        y = coff[0][1] * pow(t, 3) + coff[1][1] * pow(t, 2) + coff[2][1] * t + coff[3][1];
        SetPixel(hdc, round(x), round(y), c);
        
    }

}

void DrawBezier(HDC hdc, Point p1, Point s1, Point p2, Point s2, COLORREF c) {
    vector<vector<int>> Points = {
    {p1.x, p1.y},
    {s1.x, s1.y},
    {p2.x, p2.y},
    {s2.x, s2.y}
    };

    vector<vector<int>> coff = MultiplyMatrix(Bezier, Points);
    double x, y;
    for (double t = 0; t <= 1.0; t += 0.001) {

        x = coff[0][0] * pow(t, 3) + coff[1][0] * pow(t, 2) + coff[2][0] * t + coff[3][0];
        y = coff[0][1] * pow(t, 3) + coff[1][1] * pow(t, 2) + coff[2][1] * t + coff[3][1];
        SetPixel(hdc, round(x), round(y), c);

    }
}

double Dist(Point p1, Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

//Point GetMidPoint(Point p1, Point p2) {
//    Point mid;
//    mid.x = (p1.x + p2.x) / 2.0;
//    mid.y = (p1.y + p2.y) / 2.0;
//    return mid;
//}
//
//void RecBezier(HDC hdc, Point p1, Point p2, Point p3, Point p4, COLORREF c) {
//
//    if (Dist(p1, p4) <= 1) return;
//    Point q1 = GetMidPoint(p1, p2);
//    Point q2 = GetMidPoint(p3, p2);
//    Point q3 = GetMidPoint(p3, p4);
//    Point r1 = GetMidPoint(q1, q2);
//    Point r2 = GetMidPoint(q3, q2);
//    Point s = GetMidPoint(r1, r2);
//    SetPixel(hdc, s.x, s.y, c);
//
//    RecBezier(hdc, p1, q1, r1, s, c);
//    RecBezier(hdc, s, r2, q3, p4, c);
//
//}
//
//
//Point Rec2Bezier(double t, Point p[], int s, int e) {
//
//    if (s == e) return p[s];
//
//    return (1 - t) * Rec2Bezier(t, p, s, e - 1) + t * Rec2Bezier(t, p, s + 1, e);
//}
//
//void DrawBezier(HDC hdc, Point arr[], COLORREF c) {
//
//    for (double t = 0; t <= 1.0; t += 0.001) {
//        Point p = Rec2Bezier(t, arr, 0, 2);
//        SetPixel(hdc, p.x, p.y, c);
//    }
//}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    static int bezierCnt = 0;
    static Point points[4];

    static int hermiteCnt = 0;
    switch (uMsg) {
    case WM_LBUTTONDOWN: {
        if (bezierCnt < 4) {
            points[bezierCnt].x = LOWORD(lParam);
            points[bezierCnt].y = HIWORD(lParam);
            bezierCnt++;

            if (bezierCnt == 4) {
                hdc = GetDC(hwnd);
                DrawBezier(hdc, points[0], points[1], points[2], points[3], RGB(255, 0, 25));
                ReleaseDC(hwnd, hdc);
                bezierCnt = 0;
            }
        }
        break;
    }
    case WM_RBUTTONDOWN: {
        if (hermiteCnt < 4) {
            points[hermiteCnt].x = LOWORD(lParam);
            points[hermiteCnt].y = HIWORD(lParam);
            hermiteCnt++;

            if (hermiteCnt == 4) {
                hdc = GetDC(hwnd);
                Point s1 = 3 * (points[1] - points[0]);
                Point s2 = 3 * (points[3] - points[2]);
                DrawHermite(hdc, points[0], s1, points[3], s2, RGB(255, 0, 255));
                ReleaseDC(hwnd, hdc);
                hermiteCnt = 0;
            }
        }
        break;
    }
    case WM_COMMAND: {
       
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
    HWND hwnd = CreateWindow(L"MyClass", L"Hermite & Bezier Drawing", WS_OVERLAPPEDWINDOW,
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


