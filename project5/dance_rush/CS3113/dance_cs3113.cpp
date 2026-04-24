#include "dance_cs3113.h"

Color ColorFromHex(const char *hex)
{
    if (hex[0] == '#') hex++;
    unsigned int r = 0, g = 0, b = 0, a = 255;
    if (sscanf(hex, "%02x%02x%02x", &r, &g, &b) == 3)
        return (Color){ (unsigned char)r, (unsigned char)g,
                        (unsigned char)b, (unsigned char)a };
    if (sscanf(hex, "%02x%02x%02x%02x", &r, &g, &b, &a) == 4)
        return (Color){ (unsigned char)r, (unsigned char)g,
                        (unsigned char)b, (unsigned char)a };
    return RAYWHITE;
}

void panCamera(Camera2D *camera, const Vector2 *targetPosition)
{
    Vector2 diff   = Vector2Subtract(*targetPosition, camera->target);
    camera->target = Vector2Add(camera->target, Vector2Scale(diff, 0.1f));
}
