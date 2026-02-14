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
constexpr float DRAGONITE_SCALE = 0.40f;

constexpr float DRAGONITE_RADIUS      = 100.0f;
constexpr float DRAGONITE_ORBIT_SPEED = 2.0f;

float   gDragoniteOrbit = 0.0f;
Vector2 gDragonitePos   = ORIGIN;

// Chandelure
Texture2D gChandelure;
constexpr float CHANDELURE_SCALE     = 0.25f;
constexpr float CHANDELURE_ROT_SPEED = 80.0f;

float gChandelureRotation = 0.0f;

// Relative offset from Dragonite (still “in relation”)
constexpr float CHANDELURE_OFFSET_X = 150.0f;
constexpr float CHANDELURE_OFFSET_Y = 80.0f;

// Extra: small orbit around Dragonite so translation pattern differs
float gChandelureOrbit = 0.0f;
constexpr float CHANDELURE_ORBIT_RADIUS = 60.0f;
constexpr float CHANDELURE_ORBIT_SPEED  = 3.5f;

// Umbreon
Texture2D gUmbreon;

Direction gUmbreonDir = Bigger;

float gUmbreonScale = 0.20f;

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

    gDragonite  = LoadTexture("assets/dragonite.png");
    gChandelure = LoadTexture("assets/chandelure.png");
    gUmbreon    = LoadTexture("assets/umbreon.png");

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

    // Dragonite (orbit around screen center)
    gDragoniteOrbit += DRAGONITE_ORBIT_SPEED * deltaTime;
    float theta = gDragoniteOrbit;

    gDragonitePos.x = ORIGIN.x + DRAGONITE_RADIUS * cosf(theta);
    gDragonitePos.y = ORIGIN.y + DRAGONITE_RADIUS * sinf(theta);

    // Umbreon scaling (heartbeat)
    if (gUmbreonDir == Bigger)  gUmbreonScale += UMBREON_STEP * deltaTime;
    if (gUmbreonDir == Smaller) gUmbreonScale -= UMBREON_STEP * deltaTime;

    if (gUmbreonScale > UMBREON_MAX) gUmbreonDir = Smaller;
    if (gUmbreonScale < UMBREON_MIN) gUmbreonDir = Bigger;

    // Umbreon wobble (vertical sine)
    gUmbreonWobbleTime += UMBREON_WOBBLE_SPEED * deltaTime;

    // Umbreon lane movement (left-right + wrap + wobble)
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

    // Chandelure rotation
    gChandelureRotation += CHANDELURE_ROT_SPEED * deltaTime;
    if (gChandelureRotation >= 360.0f) gChandelureRotation -= 360.0f;

    // Chandelure translation pattern (small orbit around Dragonite + relative offset)
    gChandelureOrbit += CHANDELURE_ORBIT_SPEED * deltaTime;
}

void render()
{
    BeginDrawing();

    ClearBackground(ColorFromHex("#000000"));

    // Backgrounds (DrawTexturePro only — lecture compliant)
    {
        Texture2D bg = gBackground1;
        if (gCurrentBG == 1) bg = gBackground2;
        if (gCurrentBG == 2) bg = gBackground3;

        Rectangle bgSrc = { 0.0f, 0.0f, (float)bg.width, (float)bg.height };
        Rectangle bgDst = { 0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
        Vector2   bgOri = { 0.0f, 0.0f };

        DrawTexturePro(bg, bgSrc, bgDst, bgOri, 0.0f, WHITE);
    }

    // Dragonite (centered)
    {
        float w = gDragonite.width  * DRAGONITE_SCALE;
        float h = gDragonite.height * DRAGONITE_SCALE;

        Rectangle src = { 0.0f, 0.0f, (float)gDragonite.width, (float)gDragonite.height };
        Rectangle dst = { gDragonitePos.x - w / 2.0f, gDragonitePos.y - h / 2.0f, w, h };
        Vector2   ori = { w / 2.0f, h / 2.0f };

        DrawTexturePro(gDragonite, src, dst, ori, 0.0f, WHITE);
    }

    // Chandelure (relative to Dragonite + own orbit + rotating)
    {
        float w = gChandelure.width  * CHANDELURE_SCALE;
        float h = gChandelure.height * CHANDELURE_SCALE;

        float cx = gDragonitePos.x
                 + CHANDELURE_OFFSET_X
                 + CHANDELURE_ORBIT_RADIUS * cosf(gChandelureOrbit);

        float cy = gDragonitePos.y
                 + CHANDELURE_OFFSET_Y
                 + CHANDELURE_ORBIT_RADIUS * sinf(gChandelureOrbit);

        Rectangle src = { 0.0f, 0.0f, (float)gChandelure.width, (float)gChandelure.height };
        Rectangle dst = { cx - w / 2.0f, cy - h / 2.0f, w, h };
        Vector2   ori = { w / 2.0f, h / 2.0f };

        DrawTexturePro(gChandelure, src, dst, ori, gChandelureRotation, WHITE);
    }

    // Umbreon (centered + scaling + wobble + lane)
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
        Rectangle dst = { gUmbreonPos.x - w / 2.0f, y - h / 2.0f, w, h };
        Vector2   ori = { w / 2.0f, h / 2.0f };

        DrawTexturePro(gUmbreon, src, dst, ori, 0.0f, WHITE);
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