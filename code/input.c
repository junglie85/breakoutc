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
