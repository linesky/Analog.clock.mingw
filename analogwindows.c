
#include <windows.h>
#include <time.h>
#include <math.h>

#define ID_TIMER 1
//gcc -o analog_clock analog_clock.c -lgdi32
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawClock(HDC hdc);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "AnalogClockClass";
    
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 0)); // Fundo amarelo

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Analog Clock",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HDC hdcMem;
    static HBITMAP hbmMem;
    static HANDLE hOld;
    static RECT rcClient;

    switch (uMsg) {
    case WM_CREATE:
        SetTimer(hwnd, ID_TIMER, 5000, NULL); // Atualiza a cada 5 segundos
        GetClientRect(hwnd, &rcClient);
        hdcMem = CreateCompatibleDC(NULL);
        hbmMem = CreateCompatibleBitmap(GetDC(hwnd), rcClient.right, rcClient.bottom);
        hOld = SelectObject(hdcMem, hbmMem);
        break;

    case WM_TIMER:
        if (wParam == ID_TIMER) {
            InvalidateRect(hwnd, NULL, FALSE);
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        
        FillRect(hdcMem, &rcClient, CreateSolidBrush(RGB(255, 255, 0))); // Fundo amarelo
        DrawClock(hdcMem);
        
        BitBlt(hdc, 0, 0, rcClient.right, rcClient.bottom, hdcMem, 0, 0, SRCCOPY);
        EndPaint(hwnd, &ps);
    } break;

    case WM_DESTROY:
        KillTimer(hwnd, ID_TIMER);
        SelectObject(hdcMem, hOld);
        DeleteObject(hbmMem);
        DeleteDC(hdcMem);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void DrawClock(HDC hdc) {
    SYSTEMTIME st;
    GetLocalTime(&st);

    int cx = 200, cy = 200; // Centro do relógio
    int radius = 100;
    
    // Desenha o círculo do relógio
    SelectObject(hdc, GetStockObject(BLACK_PEN));
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Ellipse(hdc, cx - radius, cy - radius, cx + radius, cy + radius);

    // Desenha os ponteiros do relógio
    double angle;
    int x, y;

    // Ponteiro das horas
    angle = (st.wHour % 12 + st.wMinute / 60.0) * (2 * M_PI / 12) - M_PI / 2;
    x = cx + (int)(radius * 0.5 * cos(angle));
    y = cy + (int)(radius * 0.5 * sin(angle));
    MoveToEx(hdc, cx, cy, NULL);
    LineTo(hdc, x, y);

    // Ponteiro dos minutos
    angle = (st.wMinute + st.wSecond / 60.0) * (2 * M_PI / 60) - M_PI / 2;
    x = cx + (int)(radius * 0.8 * cos(angle));
    y = cy + (int)(radius * 0.8 * sin(angle));
    MoveToEx(hdc, cx, cy, NULL);
    LineTo(hdc, x, y);

    // Ponteiro dos segundos
    angle = st.wSecond * (2 * M_PI / 60) - M_PI / 2;
    x = cx + (int)(radius * 0.9 * cos(angle));
    y = cy + (int)(radius * 0.9 * sin(angle));
    MoveToEx(hdc, cx, cy, NULL);
    LineTo(hdc, x, y);
}
