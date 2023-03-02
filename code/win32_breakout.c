#include "utils.c"
#include "math.c"
#include "platform_common.c"
#include "software_renderer.c"
#include "game.c"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef struct win32_render_buffer {
    uint32_t width;
    uint32_t height;
    uint32_t *memory;
    BITMAPINFO info;
} win32_render_buffer_t;

static bool global_running;
static win32_render_buffer_t global_render_buffer;

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
        GetClientRect(window, &rect);
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

    input_t input = { 0 };

    LARGE_INTEGER counter_frequency;
    QueryPerformanceFrequency(&counter_frequency);

    LARGE_INTEGER last_counter;
    QueryPerformanceCounter(&last_counter);

    float last_dt = 0.01666f;

    global_running = true;
    while (global_running) {
        // Input.
        for (uint32_t i = 0; i < BUTTON_COUNT; ++i) { input.buttons[i].changed = false; }

        MSG message;
        while (PeekMessageW(&message, window, 0, 0, PM_REMOVE)) {

            switch (message.message) {
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP: {
                uint32_t vk_code = (uint32_t)message.wParam;
                bool was_down = (message.lParam & (1 << 30)) != 0;
                bool is_down = (message.lParam & (1 << 31)) == 0;

#define PROCESS_BUTTON(vk, button)                           \
    if (vk_code == vk) {                                     \
        input.buttons[button].changed = is_down != was_down; \
        input.buttons[button].is_down = is_down;             \
    }

                PROCESS_BUTTON(VK_LEFT, BUTTON_LEFT)
                PROCESS_BUTTON(VK_RIGHT, BUTTON_RIGHT)
                PROCESS_BUTTON(VK_UP, BUTTON_UP)
                PROCESS_BUTTON(VK_DOWN, BUTTON_DOWN)

#undef PROCESS_BUTTON

                break;
            }

            default: {
                TranslateMessage(&message);
                DispatchMessageW(&message);
            }
            }
        }

        POINT mouse_pointer;
        GetCursorPos(&mouse_pointer);
        ScreenToClient(window, &mouse_pointer);
        input.mouse.x = mouse_pointer.x;
        input.mouse.y = global_render_buffer.height - mouse_pointer.y;

        // Update.
        render_buffer_t render_buffer = { 0 };
        render_buffer.width = global_render_buffer.width;
        render_buffer.height = global_render_buffer.height;
        render_buffer.pixels = global_render_buffer.memory;

        game_update(&render_buffer, &input, last_dt);

        // Render.
        StretchDIBits(device_context,
            0, 0, (int)global_render_buffer.width, (int)global_render_buffer.height,
            0, 0, (int)global_render_buffer.width, (int)global_render_buffer.height,
            global_render_buffer.memory,
            &global_render_buffer.info,
            DIB_RGB_COLORS,
            SRCCOPY);

        LARGE_INTEGER current_counter;
        QueryPerformanceCounter(&current_counter);
        last_dt = (float)(current_counter.QuadPart - last_counter.QuadPart) / (float)counter_frequency.QuadPart;

        last_counter = current_counter;
    }

    return 0;
}
