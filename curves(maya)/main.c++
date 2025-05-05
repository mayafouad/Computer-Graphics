#include <windows.h>
#include <cmath>
#include <vector>
using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
enum CurveMode { HERMITE_MODE, BEZIER_MODE };
CurveMode currentMode = HERMITE_MODE;
struct Vector2 {
    double x, y;
    Vector2(double a = 0, double b = 0) {
        x = a; y = b;
    }
};

class Vector4 {
    double v[4];
public:
    Vector4(double a = 0, double b = 0, double c = 0, double d = 0) {
        v[0] = a; v[1] = b; v[2] = c; v[3] = d;
    }
    Vector4(double a[]) {
        memcpy(v, a, 4 * sizeof(double));
    }
    double& operator[](int i) {
        return v[i];
    }
};

class Matrix4 {
    Vector4 M[4];
public:
    Matrix4(double A[]) {
        memcpy(M, A, 16 * sizeof(double));
    }
    Vector4& operator[](int i) {
        return M[i];
    }
};

Vector4 operator*(Matrix4 M, Vector4& b) {
    Vector4 res;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            res[i] += M[i][j] * b[j];
    return res;
}

double DotProduct(Vector4& a, Vector4& b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

Vector4 GetHermiteCoeff(double x0, double s0, double x1, double s1) {
    static double H[16] = { 2,1,-2,1,-3,-2,3,-1,0,1,0,0,1,0,0,0 };
    static Matrix4 basis(H);
    Vector4 v(x0, s0, x1, s1);
    return basis * v;
}

void DrawHermiteCurve(HDC hdc, Vector2& P0, Vector2& T0, Vector2& P1, Vector2& T1, int numpoints, COLORREF color) {
    Vector4 xcoeff = GetHermiteCoeff(P0.x, T0.x, P1.x, T1.x);
    Vector4 ycoeff = GetHermiteCoeff(P0.y, T0.y, P1.y, T1.y);
    if (numpoints < 2) return;
    double dt = 1.0 / (numpoints - 1);
    for (double t = 0; t <= 1; t += dt) {
        Vector4 vt;
        vt[3] = 1;
        for (int i = 2; i >= 0; i--) vt[i] = vt[i + 1] * t;
        int x = round(DotProduct(xcoeff, vt));
        int y = round(DotProduct(ycoeff, vt));
        SetPixel(hdc, x, y, color);
    }
}

void DrawBezierCurve(HDC hdc, Vector2& P0, Vector2& P1, Vector2& P2, Vector2& P3, int numpoints){
    Vector2 T0(3 * (P1.x - P0.x), 3 * (P1.y - P0.y));
    Vector2 T1(3 * (P3.x - P2.x), 3 * (P3.y - P2.y));
    DrawHermiteCurve(hdc, P0, T0, P3, T1, numpoints, RGB(0, 150, 150));
}

int clickCount = 0;
Vector2 points[4];

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
    HDC hdc;
    switch (msg) {
    case WM_COMMAND:
        if (LOWORD(wParam) == 1)
            currentMode = HERMITE_MODE;
        else if (LOWORD(wParam) == 2)
            currentMode = BEZIER_MODE;
        break;

    case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        hdc = GetDC(hwnd);
        points[clickCount] = Vector2(x, y);
        if (clickCount == 3) {
            if (currentMode == HERMITE_MODE) {
                Vector2 T0(points[1].x - points[0].x, points[1].y - points[0].y);
                Vector2 T1(points[3].x - points[2].x, points[3].y - points[2].y);
                DrawHermiteCurve(hdc, points[0], T0, points[2], T1, 1000, RGB(255, 0, 150));
            }
            else if (currentMode == BEZIER_MODE) {
                DrawBezierCurve(hdc, points[0], points[1], points[2], points[3], 1000);
            }
        }
        clickCount = (clickCount + 1) % 4;
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "CurveDrawer";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(wc.lpszClassName, "Draw Curves", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    HMENU hMenu = CreateMenu();
    AppendMenu(hMenu, MF_STRING, 1, "Hermite Curve");
    AppendMenu(hMenu, MF_STRING, 2, "Bezier Curve");
    SetMenu(hwnd, hMenu);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
