#define UNICODE
#define _UNICODE

#define TITLE L"Pong!"
#define REAL_WINDOW_SIZE_TO_RENDER_SIZE_Y_OFFSET 39

#include "game.h"
#include "platform_common.h"
#include "render.h"

#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hDc = BeginPaint(hWnd, &ps);
            FillRect(hDc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(hWnd, &ps);
            return 0;
        }
        case WM_SIZE:
        {
            RECT rect;
            GetWindowRect(hWnd, &rect);
            renderBuffer.width = rect.right - rect.left;
            renderBuffer.height = rect.bottom - rect.top - REAL_WINDOW_SIZE_TO_RENDER_SIZE_Y_OFFSET;
            if (renderBuffer.pixels) {
                VirtualFree(renderBuffer.pixels, 0, MEM_RELEASE);
            }
            renderBuffer.pixels = VirtualAlloc(
                0,
                sizeof(unsigned int) * renderBuffer.width * renderBuffer.height,
                MEM_COMMIT | MEM_RESERVE,
                PAGE_READWRITE
            );
            renderBuffer.bitmap.bmiHeader.biSize = sizeof(renderBuffer.bitmap.bmiHeader);
            renderBuffer.bitmap.bmiHeader.biWidth = renderBuffer.width;
            renderBuffer.bitmap.bmiHeader.biHeight = renderBuffer.height;
            renderBuffer.bitmap.bmiHeader.biPlanes = 1;
            renderBuffer.bitmap.bmiHeader.biBitCount = 32;
            renderBuffer.bitmap.bmiHeader.biCompression = BI_RGB;
            renderBuffer.bitmap.bmiHeader.biSizeImage = 0;
            return 0;
        }
        case WM_CLOSE:
        {
            if (MessageBox(hWnd, L"Are you sure?", TITLE, MB_OKCANCEL) == IDOK)
            {
                DestroyWindow(hWnd);
            }
            return 0;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
        default:
        {
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }
    }
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR pCmdLine, _In_ int nCmdShow)
{
    const wchar_t windowName[] = L"Game Class";
    WNDCLASS wnd = { 0 };
    wnd.cbClsExtra = 0;
    wnd.cbWndExtra = 0;
    wnd.hCursor = LoadCursor(0, IDC_ARROW);
    wnd.hIcon = LoadIcon(0, IDI_APPLICATION);
    wnd.lpszMenuName = 0;
    wnd.style = 0;
    wnd.hbrBackground = 0;
    wnd.lpfnWndProc = WndProc;
    wnd.hInstance = hInstance;
    wnd.lpszClassName = windowName;
    if (RegisterClass(&wnd) == 0) {
        MessageBoxA(NULL, "Register class failed!", "Error!", MB_ICONERROR | MB_OK);
        return -1;
    }

    HWND hWnd = CreateWindowEx(
        0,
        windowName,
        TITLE,
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

    HDC hdc = GetDC(hWnd);
    if (hdc == 0) {
        MessageBoxA(NULL, "HDC failed!", "Error!", MB_ICONERROR | MB_OK);
        return -1;
    }

    Input input = { 0 };

    unsigned int running = 1;
    while (running) {
        // Input

        for (int i = 0; i < BUTTON_COUNT; i++) input.buttons[i].changed = 0;

        MSG msg = { 0 };
        while (PeekMessageA(&msg, hWnd, 0, 0, PM_REMOVE)) {
            
            switch (msg.message) {
                case WM_SYSKEYDOWN:
                case WM_SYSKEYUP:
                case WM_KEYDOWN:
                case WM_KEYUP: {
                    unsigned int virtualKeyCode = (unsigned int)msg.wParam;
                    unsigned int wasDown = ((msg.lParam & (1 << 30)) != 0);
                    unsigned int isDown  = ((msg.lParam & (1 << 31)) == 0);

                    #define ProccessButton(vk, b) \
                    if (virtualKeyCode == vk) {\
                        input.buttons[b].changed = isDown != input.buttons[b].isDown;\
                        input.buttons[b].isDown = isDown;\
                    }

                    ProccessButton(VK_LEFT, BUTTON_LEFT);
                    ProccessButton(VK_RIGHT, BUTTON_RIGHT);
                    ProccessButton(VK_UP, BUTTON_UP);
                    ProccessButton(VK_DOWN, BUTTON_DOWN);

                } break;
                default: {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
        }

        // Simulation
        SimulateGame(&input);

        // Render  
        StretchDIBits(
            hdc,
            0, 0,
            renderBuffer.width, renderBuffer.height,
            0, 0,
            renderBuffer.width, renderBuffer.height,
            renderBuffer.pixels,
            &renderBuffer.bitmap,
            DIB_RGB_COLORS,
            SRCCOPY
        );
    }

    return 0;
}