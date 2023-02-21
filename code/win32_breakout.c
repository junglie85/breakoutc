#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdbool.h>
#include <stdint.h>

#define UNUSED(X) ((void)(X))

struct win32_render_buffer {
    uint32_t width;
    uint32_t height;
    uint32_t *memory;
    BITMAPINFO info;
};

static bool global_running;
static struct win32_render_buffer global_render_buffer;

static LRESULT win32_window_procedure(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
    LRESULT result = 0;

    switch (message) {
    case WM_CLOSE:
    case WM_DESTROY: {
        global_running = false;
        break;
    }

    case WM_SIZE: {
        RECT rect;
        GetWindowRect(window, &rect);
        global_render_buffer.width = rect.right - rect.left;
        global_render_buffer.height = rect.bottom - rect.top;

        if (global_render_buffer.memory) {
            VirtualFree(global_render_buffer.memory, 0, MEM_RELEASE);
        }

        global_render_buffer.memory = VirtualAlloc(0,
            sizeof(uint32_t) * global_render_buffer.width * global_render_buffer.height,
            MEM_COMMIT | MEM_RESERVE,
            PAGE_READWRITE);

        global_render_buffer.info.bmiHeader.biSize = sizeof(global_render_buffer.info.bmiHeader);
        global_render_buffer.info.bmiHeader.biWidth = global_render_buffer.width;
        global_render_buffer.info.bmiHeader.biHeight = global_render_buffer.height;
        global_render_buffer.info.bmiHeader.biPlanes = 1;
        global_render_buffer.info.bmiHeader.biBitCount = 32;
        global_render_buffer.info.bmiHeader.biCompression = BI_RGB;

        break;
    }

    default:
        result = DefWindowProcW(window, message, w_param, l_param);
    }

    return result;
}

static void clear_screen(struct win32_render_buffer *render_buffer, uint32_t color)
{
    uint32_t *pixel = render_buffer->memory;
    for (uint32_t y = 0; y < render_buffer->height; ++y) {
        for (uint32_t x = 0; x < render_buffer->width; ++x) {
            *pixel++ = color;
        }
    }
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

    HDC device_context = GetDC(window);

    global_running = true;
    while (global_running) {
        // Input.
        MSG message;
        while (PeekMessageW(&message, window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }

        // Update.
        clear_screen(&global_render_buffer, 0x00ff4400);

        // Render.
        StretchDIBits(device_context,
            0, 0, (int)global_render_buffer.width, (int)global_render_buffer.height,
            0, 0, (int)global_render_buffer.width, (int)global_render_buffer.height,
            global_render_buffer.memory,
            &global_render_buffer.info,
            DIB_RGB_COLORS,
            SRCCOPY);
    }

    return 0;
}
