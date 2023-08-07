#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#define TITLE L"Pong!"
#define REAL_WINDOW_SIZE_TO_RENDER_SIZE_X_OFFSET 16
#define REAL_WINDOW_SIZE_TO_RENDER_SIZE_Y_OFFSET 39

#include "game.h"
#include "platform_common.h"
#include "render.h"
#include "win32_render.h"
#include "win32_input.h"

#include <Windows.h>


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void ProccessButtons(Input* input, int vkCode, int wasDown, int isDown);

// TODO: move some stuff to heap
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
    
    // Configs
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));
    HFONT hFont = CreateFontA(10, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
    SendMessage(hWnd, WM_SETFONT, hFont, TRUE);

    // Intialize game
    MSG msg = { 0 };
    while (PeekMessageA(&msg, hWnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    initialScreenSize.width = renderBuffer.width;
    initialScreenSize.height = renderBuffer.height;
    CreateGame();
    StretchDIBits(
        hdc,
        0, 0,
        renderBuffer.width, renderBuffer.height,
        0, 0,
        renderBuffer.width, renderBuffer.height,
        renderBuffer.pixels,
        &win32RenderBuffer.bitmap,
        DIB_RGB_COLORS,
        SRCCOPY
    );


    LARGE_INTEGER lastTime;
    QueryPerformanceCounter(&lastTime);
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    float lastDeltaTime = 0.01666f; // 60 fps initially

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
                    ProccessButtons(&input, virtualKeyCode, wasDown, isDown);
                } break;
                default: {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
        }

        // Simulation
        SimulateGame(&input, lastDeltaTime);

        // Render  
        StretchDIBits(
            hdc,
            0, 0,
            renderBuffer.width, renderBuffer.height,
            0, 0,
            renderBuffer.width, renderBuffer.height,
            renderBuffer.pixels,
            &win32RenderBuffer.bitmap,
            DIB_RGB_COLORS,
            SRCCOPY
        );
        for (int i = 0; i < TEXT_COUNT; i++) {
            Text text = uiTexts[i];
            if (text.active) {
                LPCWSTR convertedText[4096] = { 0 };
                MultiByteToWideChar(CP_ACP, 0, text.text, -1, &convertedText, 4096);
                if (convertedText != NULL) {
                    RECT rect = { .left = text.upLeftX, .top = text.upLeftY, .right = text.downRightX, .bottom = text.downRightY };
                    // TODO: fix flickering caused by StretchDIBits not considering text every frame
                    DrawText(hdc, convertedText, text.textLength, &rect, DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_EXPANDTABS); 
                }
            }
        }

        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        QueryPerformanceCounter(&currentTime);
        lastDeltaTime = (float) (currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;
        lastTime = currentTime;
    }

    return 0;
}

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
        renderBuffer.width = rect.right - rect.left - REAL_WINDOW_SIZE_TO_RENDER_SIZE_X_OFFSET;
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
        win32RenderBuffer.bitmap.bmiHeader.biSize = sizeof(win32RenderBuffer.bitmap.bmiHeader);
        win32RenderBuffer.bitmap.bmiHeader.biWidth = renderBuffer.width;
        win32RenderBuffer.bitmap.bmiHeader.biHeight = renderBuffer.height;
        win32RenderBuffer.bitmap.bmiHeader.biPlanes = 1;
        win32RenderBuffer.bitmap.bmiHeader.biBitCount = 32;
        win32RenderBuffer.bitmap.bmiHeader.biCompression = BI_RGB;
        win32RenderBuffer.bitmap.bmiHeader.biSizeImage = 0;
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

void ProccessButtons(Input* input, int vkCode, int wasDown, int isDown) {
    #define ProccessButton(b) \
    input->buttons[b].changed = isDown != input->buttons[b].isDown;\
    input->buttons[b].isDown = isDown;\

    switch (vkCode) {
        case VK_A:
        case VK_LEFT: {
            ProccessButton(BUTTON_LEFT);
        } break;
        case VK_D:
        case VK_RIGHT: {
            ProccessButton(BUTTON_RIGHT);
        } break;
        case VK_W:
        case VK_UP: {
            ProccessButton(BUTTON_UP);
        } break;
        case VK_S:
        case VK_DOWN: {
            ProccessButton(BUTTON_DOWN);
        } break;
        case VK_P: {
            ProccessButton(BUTTON_PAUSE);
        } break;
        case VK_R: {
            ProccessButton(BUTTON_RESTART);
        }
        default: {
            
        } break;
    }
}