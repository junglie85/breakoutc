v2 player_p = { 0 };

void game_update(render_buffer_t *render_buffer, input_t *input, float dt)
{
    float speed = 100.0f;
    if (input_button_held(input, BUTTON_LEFT)) { player_p.x -= speed * dt; }
    if (input_button_held(input, BUTTON_RIGHT)) { player_p.x += speed * dt; }
    if (input_button_held(input, BUTTON_UP)) { player_p.y += speed * dt; }
    if (input_button_held(input, BUTTON_DOWN)) { player_p.y -= speed * dt; }

    clear_screen(render_buffer, 0x551100);
    draw_rect(render_buffer, player_p, (v2) { 2.0f, 2.0f }, 0xffff00);
}
