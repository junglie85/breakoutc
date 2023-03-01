static void clear_screen(render_buffer_t *render_buffer, uint32_t color)
{
    uint32_t *pixel = render_buffer->pixels;
    for (uint32_t y = 0; y < render_buffer->height; ++y) {
        for (uint32_t x = 0; x < render_buffer->width; ++x) {
            *pixel++ = color;
        }
    }
}

static void draw_rect_in_pixels(render_buffer_t *render_buffer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color)
{
    x0 = clampi(0, x0, render_buffer->width);
    y0 = clampi(0, y0, render_buffer->height);
    x1 = clampi(0, x1, render_buffer->width);
    y1 = clampi(0, y1, render_buffer->height);

    for (int32_t y = y0; y < y1; ++y) {
        uint32_t *pixel = render_buffer->pixels + x0 + render_buffer->width * y;
        for (int32_t x = x0; x < x1; ++x) {
            *pixel++ = color;
        }
    }
}

static void draw_rect(render_buffer_t *render_buffer, v2 pos, v2 size, uint32_t color)
{
    float aspect_ratio = 16.0f / 9.0f;
    float aspect_multiplier = (float)render_buffer->height;
    if ((float)render_buffer->width / (float)render_buffer->height < aspect_ratio) {
        aspect_multiplier = (float)render_buffer->width / aspect_ratio;
    }

    float scale = 0.01f;
    v2 half_size = { size.x * 0.5f, size.y * 0.5f };
    half_size.x *= aspect_multiplier * scale;
    half_size.y *= aspect_multiplier * scale;

    pos.x *= aspect_multiplier * scale;
    pos.y *= aspect_multiplier * scale;

    pos.x += (float)render_buffer->width * 0.5f;
    pos.y += (float)render_buffer->height * 0.5f;

    int32_t x0 = (int32_t)(pos.x - half_size.x);
    int32_t y0 = (int32_t)(pos.y - half_size.y);
    int32_t x1 = (int32_t)(pos.x + half_size.x);
    int32_t y1 = (int32_t)(pos.y + half_size.y);

    draw_rect_in_pixels(render_buffer, x0, y0, x1, y1, color);
}
