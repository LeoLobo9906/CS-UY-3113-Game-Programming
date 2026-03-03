#include "cs3113.h"
#include <math.h>

Color ColorFromHex(const char *hex)
{
    if (hex[0] == '#') hex++;

    unsigned int r = 0,
                 g = 0,
                 b = 0,
                 a = 255;

    if (sscanf(hex, "%02x%02x%02x", &r, &g, &b) == 3) {
        return (Color){ (unsigned char)r,
                        (unsigned char)g,
                        (unsigned char)b,
                        (unsigned char)a };
    }

    if (sscanf(hex, "%02x%02x%02x%02x", &r, &g, &b, &a) == 4) {
        return (Color){ (unsigned char)r,
                        (unsigned char)g,
                        (unsigned char)b,
                        (unsigned char)a };
    }

    return RAYWHITE;
}

float GetLength(Vector2 *v)
{
    return sqrtf(v->x * v->x + v->y * v->y);
}

void Normalise(Vector2 *v)
{
    float len = GetLength(v);
    if (len > 0.0f)
    {
        v->x /= len;
        v->y /= len;
    }
}