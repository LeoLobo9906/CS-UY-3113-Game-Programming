/**
* Author: Leonardo Villalobos
* Assignment: Pong Clone
* Date due: [02/14/2026]
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/cs3113.h"
#include <math.h>

enum Direction { Bigger, Smaller };
enum UmbreonLane { BOTTOM_LANE, TOP_LANE };

constexpr int SCREEN_WIDTH  = (int)(800 * 1.5f);
constexpr int SCREEN_HEIGHT = (int)(450 * 1.5f);
constexpr int FPS           = 60;

constexpr float RADIUS      = 100.0f;
constexpr float ORBIT_SPEED = 2.0f;


constexpr Vector2 ORIGIN = {
    SCREEN_WIDTH  / 2.0f,
    SCREEN_HEIGHT / 2.0f
};

// Global Variables
AppStatus gAppStatus = RUNNING;

// Delta time
float gPreviousTicks = 0.0f;

// Backgrounds
Texture2D gBackground1;
Texture2D gBackground2;
Texture2D gBackground3;

int   gCurrentBG = 0;
float gBGTime    = 0.0f;
constexpr float BG_SECONDS = 2.0f;

// Dragonite
Texture2D gDragonite;
constexpr float DRAGONITE_SCALE = 0.4f;
float   gOrbitLocation = 0.0f;
Vector2 gPosition      = ORIGIN;

// Chandelure
Texture2D gChandelure;
float gChandelureRotation = 0.0f;
constexpr float CHANDELURE_SCALE     = 0.25f;
constexpr float CHANDELURE_ROT_SPEED = 80.0f;

constexpr float CHANDELURE_OFFSET_X = 150.0f;
constexpr float CHANDELURE_OFFSET_Y = 80.0f;

// Umbreon
Texture2D gUmbreon;

Direction gUmbreonDir = Bigger;

float gUmbreonScale   = 0.20f;

constexpr float UMBREON_MIN  = 0.16f;
constexpr float UMBREON_MAX  = 0.26f;
constexpr float UMBREON_STEP = 0.20f;

UmbreonLane gUmbreonLane = BOTTOM_LANE;
Vector2 gUmbreonPos = { 0.0f, 0.0f };
constexpr float UMBREON_SPEED = 350.0f;

float gUmbreonWobbleTime = 0.0f;
constexpr float UMBREON_WOBBLE_SPEED = 8.0f;
constexpr float UMBREON_WOBBLE_AMP   = 36.0f;


void initialise();
void processInput();
void update();
void render();
void shutdown();

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project 2: Simple 2D Scene");
    SetTargetFPS(FPS);

    gDragonite   = LoadTexture("assets/dragonite.png");
    gChandelure  = LoadTexture("assets/chandelure.png");
    gUmbreon     = LoadTexture("assets/umbreon.png");

    gBackground1 = LoadTexture("assets/background_1.png");
    gBackground2 = LoadTexture("assets/background_2.png");
    gBackground3 = LoadTexture("assets/background_3.png");

    // Starts Umbreon off-screen
    gUmbreonPos.x = -200.0f;
}

void processInput()
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    // Delta time
    float ticks     = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    // Background switching
    gBGTime += 1.0f * deltaTime;
    if (gBGTime >= BG_SECONDS) {
        gBGTime = 0.0f;
        gCurrentBG++;
        if (gCurrentBG > 2) gCurrentBG = 0;
    }

    // Dragonite
    gOrbitLocation += ORBIT_SPEED * deltaTime;
    float theta = gOrbitLocation;
    gPosition.x = ORIGIN.x + RADIUS * cosf(theta);
    gPosition.y = ORIGIN.y + RADIUS * sinf(theta);

    // Umbreon
    if (gUmbreonDir == Bigger)  gUmbreonScale += UMBREON_STEP * deltaTime;
    if (gUmbreonDir == Smaller) gUmbreonScale -= UMBREON_STEP * deltaTime;

    if (gUmbreonScale > UMBREON_MAX) gUmbreonDir = Smaller;
    if (gUmbreonScale < UMBREON_MIN) gUmbreonDir = Bigger;

    // Umbreon
    gUmbreonWobbleTime += UMBREON_WOBBLE_SPEED * deltaTime;

    // Umbreon
    if (gUmbreonLane == BOTTOM_LANE) {
        gUmbreonPos.x += UMBREON_SPEED * deltaTime;

        if (gUmbreonPos.x > SCREEN_WIDTH + 200.0f) {
            gUmbreonLane  = TOP_LANE;
            gUmbreonPos.x = SCREEN_WIDTH + 200.0f;
        }
    }
    else {
        gUmbreonPos.x -= UMBREON_SPEED * deltaTime;

        if (gUmbreonPos.x < -200.0f) {
            gUmbreonLane  = BOTTOM_LANE;
            gUmbreonPos.x = -200.0f;
        }
    }

    // Chandelure
    gChandelureRotation += CHANDELURE_ROT_SPEED * deltaTime;
    if (gChandelureRotation >= 360.0f) gChandelureRotation -= 360.0f;
}

void render()
{
    BeginDrawing();

    ClearBackground(ColorFromHex("#000000"));

    // Background
    if (gCurrentBG == 0) DrawTexture(gBackground1, 0, 0, WHITE);
    if (gCurrentBG == 1) DrawTexture(gBackground2, 0, 0, WHITE);
    if (gCurrentBG == 2) DrawTexture(gBackground3, 0, 0, WHITE);

    // Dragonite
    {
        Rectangle src = { 0.0f, 0.0f, (float)gDragonite.width, (float)gDragonite.height };

        Rectangle dst = {
            gPosition.x,
            gPosition.y,
            gDragonite.width  * DRAGONITE_SCALE,
            gDragonite.height * DRAGONITE_SCALE
        };

        Vector2 origin = { dst.width / 2.0f, dst.height / 2.0f };

        DrawTexturePro(gDragonite, src, dst, origin, 0.0f, WHITE);
    }

    // Chandelure
    {
        float w = gChandelure.width  * CHANDELURE_SCALE;
        float h = gChandelure.height * CHANDELURE_SCALE;

        float cx = gPosition.x + CHANDELURE_OFFSET_X;
        float cy = gPosition.y + CHANDELURE_OFFSET_Y;

        Rectangle src = { 0.0f, 0.0f, (float)gChandelure.width, (float)gChandelure.height };
        Rectangle dst = { cx, cy, w, h };
        Vector2 origin = { w / 2.0f, h / 2.0f };

        DrawTexturePro(gChandelure, src, dst, origin, gChandelureRotation, WHITE);
    }

    // Umbreon
    {
        float margin = 20.0f;

        float w = gUmbreon.width  * gUmbreonScale;
        float h = gUmbreon.height * gUmbreonScale;

        float y = 0.0f;
        if (gUmbreonLane == BOTTOM_LANE) {
            y = SCREEN_HEIGHT - margin - h / 2.0f;
        } else {
            y = margin + h / 2.0f;
        }

        y += UMBREON_WOBBLE_AMP * sinf(gUmbreonWobbleTime);

        Rectangle src = { 0.0f, 0.0f, (float)gUmbreon.width, (float)gUmbreon.height };
        Rectangle dst = { gUmbreonPos.x, y, w, h };
        Vector2 origin = { w / 2.0f, h / 2.0f };

        DrawTexturePro(gUmbreon, src, dst, origin, 0.0f, WHITE);
    }

    EndDrawing();
}

void shutdown()
{
    UnloadTexture(gDragonite);
    UnloadTexture(gChandelure);
    UnloadTexture(gUmbreon);

    UnloadTexture(gBackground1);
    UnloadTexture(gBackground2);
    UnloadTexture(gBackground3);

    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();
    return 0;
}
