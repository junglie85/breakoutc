#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdbool.h>

#define UNUSED(X) ((void)(X))

static bool running;

static LRESULT win32_window_procedure(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
    LRESULT result = 0;

    switch (message) {
    case WM_CLOSE:
    case WM_DESTROY:
        running = false;
        break;

    default:
        result = DefWindowProcW(window, message, w_param, l_param);
    }

    return result;
}

int WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR cmdline, int showcmd)
{
    UNUSED(previous_instance);
    UNUSED(cmdline);
    UNUSED(showcmd);

    WNDCLASSW window_class = { 0 };
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpfnWndProc = win32_window_procedure;
    window_class.hInstance = instance;
    window_class.lpszClassName = L"GameWindowClass";

    if (!RegisterClassW(&window_class)) {
        // todo: logging.
        return -1;
    }

    HWND window = CreateWindowExW(0,
        window_class.lpszClassName,
        L"Breakout",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1280,
        720,
        0,
        0,
        instance,
        0);

    if (!window) {
        // todo: logging.
        return -1;
    }

    running = true;
    while (running) {
        MSG message;
        while (PeekMessageW(&message, window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }
    }

    return 0;
}
