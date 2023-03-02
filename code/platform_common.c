typedef struct render_buffer {
    uint32_t width;
    uint32_t height;
    uint32_t *pixels;
} render_buffer_t;

typedef struct button {
    bool is_down;
    bool changed;
} button_t;

enum {
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_UP,
    BUTTON_DOWN,

    BUTTON_COUNT,
};

typedef struct input {
    v2i mouse;
    button_t buttons[BUTTON_COUNT];
} input_t;

static bool input_button_pressed(input_t *input, uint32_t button)
{
    bool pressed = false;

    if (button < BUTTON_COUNT) {
        pressed = input->buttons[button].is_down && input->buttons[button].changed;
    }

    return pressed;
}

static bool input_button_released(input_t *input, uint32_t button)
{
    bool released = false;

    if (button < BUTTON_COUNT) {
        released = !input->buttons[button].is_down && input->buttons[button].changed;
    }

    return released;
}

static bool input_button_held(input_t *input, uint32_t button)
{
    bool held = false;

    if (button < BUTTON_COUNT) {
        held = input->buttons[button].is_down;
    }

    return held;
}
