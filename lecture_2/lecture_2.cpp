#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Enums
enum AppStatus { TERMINATED, RUNNING };
enum Direction { Bigger, Smaller };

// Global Constants
constexpr int SCREEN_WIDTH  = (int)(800 * 1.5f),
              SCREEN_HEIGHT = (int)(450 * 1.5f),
              FPS           = 60,
              SIDES         = 3;

constexpr float RADIUS          = 100.0f, // radius of the orbit
                ORBIT_SPEED     = 0.05f,  // the speed at which the triangle will travel its orbit
                BASE_SIZE       = 50.0f,  // the size of the triangle when it's not being scaled
                MAX_AMPLITUDE   = 10.0f,  // by how much the triangle will be expanding/contracting
                PULSE_SPEED     = 100.0f, // how fast the triangle is going to be "pulsing"
                PULSE_INCREMENT = 10.0f;  // the current value we're scaling by

constexpr Vector2 ORIGIN = {
    SCREEN_WIDTH / 2.0f,
    SCREEN_HEIGHT / 2.0f
};

// Global Variables
AppStatus gAppStatus = RUNNING;
Direction curdirection = Bigger;

float gScaleFactor = BASE_SIZE;
float gAngle = 0.0f;
float gPulseTime = 0.0f;
Vector2 gPosition = ORIGIN;

float gOrbitLocation = 0.0f;

// Function Declarations
Color ColorFromHex(const char *hex);
void initialise();
void processInput();
void update();
void render();
void shutdown();

// You can ignore this function, it's just to get nice colours :
Color ColorFromHex(const char *hex)
{
    // Skip leading '#', if present
    if (hex[0] == '#') hex++;

    // Default alpha = 255 (opaque)
    unsigned int r = 0, g = 0, b = 0, a = 255;

    // 6-digit form: RRGGBB
    if (sscanf(hex, "%02x%02x%02x", &r, &g, &b) == 3) {
        Color c;
        c.r = (unsigned char)r;
        c.g = (unsigned char)g;
        c.b = (unsigned char)b;
        c.a = (unsigned char)a;
        return c;
    }

    // 8-digit form: RRGGBBAA
    if (sscanf(hex, "%02x%02x%02x%02x", &r, &g, &b, &a) == 4) {
        Color c;
        c.r = (unsigned char)r;
        c.g = (unsigned char)g;
        c.b = (unsigned char)b;
        c.a = (unsigned char)a;
        return c;
    }

    // Fallback – return white so you notice something went wrong
    return RAYWHITE;
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Funny Heartbeat");
    SetTargetFPS(FPS);
}

void processInput()
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    if (curdirection == Smaller) {
        gScaleFactor -= 1.0f;
    }
    if (curdirection == Bigger) {
        gScaleFactor += 1.0f;
    }

    if (gScaleFactor > 75.0f) {
        curdirection = Smaller;
    }
    if (gScaleFactor < 25.0f) {
        curdirection = Bigger;
    }

    gOrbitLocation += ORBIT_SPEED;
    float theta = gOrbitLocation;
    gPosition.x = ORIGIN.x + RADIUS * cosf(theta);
    gPosition.y = ORIGIN.y + RADIUS * sinf(theta);
}

void render()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawPoly(
        gPosition,
        SIDES,
        gScaleFactor,
        gAngle,
        ColorFromHex("#F88379")
    );

    EndDrawing();
}

void shutdown()
{
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING) {
        processInput();
        update();
        render();
    }

    shutdown();
    return 0;
}
