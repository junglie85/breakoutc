v2 ball_p = { 0 };
v2 ball_dp = { 0 };
v2 ball_size = { 0 };

v2 player_p = { 0 };
v2 player_dp = { 0 };
v2 player_size = { 0 };

v2 block_size = { 0 };

typedef struct {
    v2 p;
    int32_t life;
    uint32_t color;
} block_t;

block_t blocks[256];

v2 arena_size = { 0 };

bool initialised = false;

void game_update(render_buffer_t *render_buffer, input_t *input, float dt)
{
    if (!initialised) {
        initialised = true;
        ball_p.x = 75.0f;
        ball_p.y = 40.0f;
        ball_dp.y = -50.0f;
        ball_size = (v2) { 1.5f, 1.5f };

        player_p.y = -40.0f;
        player_size = (v2) { 20.0f, 4.0f };

        arena_size = (v2) { 170.0f, 90.0f };

        block_size = (v2) { 8.0f, 4.0f };

        int32_t num_x = 18;
        int32_t num_y = 9;
        float x_offset = (float)(block_size.x * 0.5f * num_x);
        float y_offset = (float)(block_size.y * 0.5f * num_y) - 15.0f;
        int32_t next_block = 0;
        for (int32_t y = 0; y < num_y; ++y) {
            for (int32_t x = 0; x < num_x; ++x) {
                block_t *block = &blocks[next_block++];
                if (next_block >= ARRAY_COUNT(blocks)) {
                    next_block = 0;
                }

                block->life = 1;
                block->p.x = x * block_size.x - x_offset;
                block->p.y = y * block_size.y - y_offset;
                block->color = make_grey_color((uint8_t)((x + y) * 5));
            }
        }
    }

    v2 future_player_p = pixels_to_world(render_buffer, input->mouse);
    v2 future_ball_p = add_v2(ball_p, mul_v2(ball_dp, dt));

    aabb_t ball = { future_ball_p.x, future_ball_p.y, ball_size.x, ball_size.y };
    aabb_t player = { player_p.x, player_p.y, player_size.x, player_size.y };
    if (ball_dp.y < 0.0f && collision(ball, player)) {
        ball_dp.x += clampf(-100.0f, player_dp.x, 100.0f);
        ball_dp.y *= -1.0f;
        future_ball_p.y = player_p.y + player_size.y * 0.5f;
    } else if (future_ball_p.x + ball_size.x * 0.5f > arena_size.x * 0.5f) {
        future_ball_p.x = arena_size.x * 0.5f - ball_size.x * 0.5f;
        ball_dp.x *= -1.0f;
    } else if (future_ball_p.x - ball_size.x * 0.5f < -arena_size.x * 0.5f) {
        future_ball_p.x = -arena_size.x * 0.5f + ball_size.x * 0.5f;
        ball_dp.x *= -1.0f;
    }

    if (future_ball_p.y + ball_size.y * 0.5f > arena_size.y * 0.5f) {
        future_ball_p.y = arena_size.y * 0.5f - ball_size.y * 0.5f;
        ball_dp.y *= -1.0f;
    }

    clear_screen(render_buffer, 0x220500);
    draw_rect(render_buffer, (v2) { 0.0f, 0.0f }, arena_size, 0x551100);

    for (block_t *block = &blocks[0]; block != &blocks[ARRAY_COUNT(blocks)]; ++block) {
        if (!block->life) { continue; }

        // todo: split collision detection and rendering into seperate loops.
        aabb_t block_aabb = { block->p.x, block->p.y, block_size.x, block_size.y };
        if (collision(ball, block_aabb)) {
            float p_diff = absf(future_ball_p.x - ball_p.x);
            if (p_diff != 0.0f) {
                float collision_point = ball_dp.x > 0.0f ? block->p.x - (block_size.x * 0.5f) - (ball_size.x * 0.5f) : block->p.x + (block_size.x * 0.5f) + (ball_size.x * 0.5f);
                float t_x = (collision_point - ball_p.x) / p_diff;
                if (t_x >= 0.0f && t_x <= 1.0f) {
                    float target_y = lerp(ball_p.y, future_ball_p.y, t_x);
                    if (target_y + (ball_size.y * 0.5f) > block->p.y - (block_size.y * 0.5f)
                        && target_y - (ball_size.y * 0.5f) < block->p.y + (block_size.y * 0.5f)) {
                        future_ball_p.x = lerp(ball_p.x, future_ball_p.x, t_x);
                        ball_dp.x *= -1.0f;
                        --block->life;
                    }
                }
            }

            p_diff = absf(future_ball_p.y - ball_p.y);
            if (p_diff != 0.0f) {
                float collision_point = ball_dp.y > 0.0f ? block->p.y - (block_size.y * 0.5f) - (ball_size.y * 0.5f) : block->p.y + (block_size.y * 0.5f) + (ball_size.y * 0.5f);
                float t_y = (collision_point - ball_p.y) / p_diff;
                if (t_y >= 0.0f && t_y <= 1.0f) {
                    float target_x = lerp(ball_p.x, future_ball_p.x, t_y);
                    if (target_x + (ball_size.x * 0.5f) > block->p.x - (block_size.x * 0.5f)
                        && target_x - (ball_size.x * 0.5f) < block->p.x + (block_size.x * 0.5f)) {
                        future_ball_p.y = lerp(ball_p.y, future_ball_p.y, t_y);
                        ball_dp.y *= -1.0f;
                        --block->life;
                    }
                }
            }
        }

        draw_rect(render_buffer, block->p, block_size, block->color);
    }

    player_dp.x = (future_player_p.x - player_p.x) / dt;
    player_p.x = future_player_p.x;
    ball_p = future_ball_p;

    draw_rect(render_buffer, ball_p, ball_size, 0x00ffff);
    draw_rect(render_buffer, player_p, player_size, 0x00ff00);
}
