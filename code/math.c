#define ARRAY_COUNT(X) (sizeof((X)) / sizeof((X)[0]))

static float clampf(float min, float val, float max)
{
    if (val < min) { return min; }
    if (val > max) { return max; }
    return val;
}

static uint32_t clampi(int32_t min, int32_t val, int32_t max)
{
    if (val < min) { return min; }
    if (val > max) { return max; }
    return val;
}

static inline float lerp(float a, float b, float t)
{
    float result = (1.0f - t) * a + t * b;
    return result;
}

static inline float absf(float x)
{
    if (x < 0.0f) { return -x; }
    return x;
}

static inline uint32_t make_grey_color(uint8_t grey)
{
    return (grey << 0) | (grey << 8) | (grey << 16);
}

// O--------------------------------------------------------------------------O
// | vector2f                                                                 |
// O--------------------------------------------------------------------------O

typedef union vector2f {
    struct {
        float x;
        float y;
    };

    float i[2];
} v2;

inline v2 add_v2(v2 a, v2 b)
{
    v2 result = { a.x + b.x, a.y + b.y };
    return result;
}

inline v2 mul_v2(v2 a, float s)
{
    v2 result = { a.x * s, a.y * s };
    return result;
}

// O--------------------------------------------------------------------------O
// | vector2i                                                                 |
// O--------------------------------------------------------------------------O

typedef union vector2i {
    struct {
        int32_t x;
        int32_t y;
    };

    int32_t i[2];
} v2i;

// O--------------------------------------------------------------------------O
// | AABB                                                                     |
// O--------------------------------------------------------------------------O

typedef struct aabb {
    float x;
    float y;
    float width;
    float height;
} aabb_t;

inline bool collision(aabb_t a, aabb_t b)
{
    v2 a_half_size = (v2) { a.width * 0.5f, a.height * 0.5f };
    v2 b_half_size = (v2) { b.width * 0.5f, b.height * 0.5f };
    bool collision = a.x + a_half_size.x > b.x - b_half_size.x
        && a.x - a_half_size.x < b.x + b_half_size.x
        && a.y + a_half_size.y > b.y - b_half_size.y
        && a.y - a_half_size.y < b.y + b_half_size.y;

    return collision;
}
