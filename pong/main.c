#define UNICODE
#define _UNICODE

#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR pCmdLine, _In_ int nCmdShow)
{
    const wchar_t windowName[] = L"Game Class";
    WNDCLASS wnd = { 0 };
    wnd.cbClsExtra      = 0;
    wnd.cbWndExtra      = 0;
    wnd.hCursor         = LoadCursor(0, IDC_ARROW);
    wnd.hIcon           = LoadIcon(0, IDI_APPLICATION);
    wnd.lpszMenuName    = 0;
    wnd.style           = 0;
    wnd.hbrBackground   = 0;
    wnd.lpfnWndProc     = WndProc;
    wnd.hInstance       = hInstance;
    wnd.lpszClassName   = windowName;
    if (RegisterClass(&wnd) == 0) {
        MessageBoxA(NULL, "Register class failed!", "Error!", MB_ICONERROR | MB_OK);
        return -1;
    }

    HWND hWnd = CreateWindowEx(
        0,
        windowName,
        L"Pong!",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    if (hWnd == NULL) {
        MessageBoxA(NULL, "Create window failed!", "Error!", MB_ICONERROR | MB_OK);
        return -1;
    }

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hDc = BeginPaint(hWnd, &ps);
            FillRect(hDc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(hWnd, &ps);
            return 0;
        }
        default:
        {
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }
    }
    return 0;
}