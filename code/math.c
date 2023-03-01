static uint32_t clampi(int32_t min, int32_t val, int32_t max)
{
    if (val < min) { return min; }
    if (val > max) { return max; }
    return val;
}

/* Vector2 */

typedef union vector2 {
    struct {
        float x;
        float y;
    };

    float i[2];
} v2;
