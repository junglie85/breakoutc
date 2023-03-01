#ifndef BREAKOUT_H
#define BREAKOUT_H

#include <stdbool.h>
#include <stdint.h>

#define UNUSED(X) ((void)(X))

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
    int32_t mouse_x;
    int32_t mouse_y;

    button_t buttons[BUTTON_COUNT];
} input_t;

void game_update(render_buffer_t *render_buffer, input_t *input, float dt);

#endif // BREAKOUT_H
