/**
* Author: Leonardo Vilallobos
* Assignment: Lunar Lander
* Date due: 3/14/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/cs3113.h"
#include "CS3113/Entity.h"
#include <math.h>

constexpr int SCREEN_WIDTH  = 1200;
constexpr int SCREEN_HEIGHT = 700;
constexpr int FPS           = 60;

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Physics
constexpr float GRAVITY        = 30.0f;
constexpr float HORIZ_ACCEL    = 500.0f;
constexpr float ACCEL_DRAG     = 0.99f;
constexpr float MAX_FALL_SPEED = 80.0f;

// Fuel
constexpr float FUEL_MAX     = 150.0f;
constexpr float FUEL_PER_KEY = 1.0f;

// Platform geometry
constexpr int   NUM_STATIC_PLATFORMS = 3;    // 3 fixed islands
constexpr float PLAT_TILE_W          = 65.0f;
constexpr float PLAT_TILE_H          = 28.0f;
constexpr float MOVING_PLAT_SPEED    = 90.0f;

constexpr int NUM_COLLISION_TILES = 4;
constexpr int NUM_VISUAL_TILES    = 8;

// Airplane
constexpr float AIRPLANE_SPEED   = 280.0f;
constexpr float AIRPLANE_W       = 120.0f;
constexpr float AIRPLANE_H       = 80.0f;
constexpr float AIRPLANE_Y       = SCREEN_HEIGHT * 0.30f;
constexpr float AIRPLANE_START_X = -AIRPLANE_W / 2.0f - 10.0f;

constexpr float BIRD_SPAWN_Y   = 50.0f;
constexpr float SAFE_FALL_DIST = 180.0f;

constexpr char BG_FP[]       = "assets/blueskybackground.png";
constexpr char DIRT_FP[]     = "assets/dirtpng.png";
constexpr char TREE_FP[]     = "assets/tree.png";
constexpr char BIRD_FP[]     = "assets/bird.png";
constexpr char AIRPLANE_FP[] = "assets/airplane.png";

enum GameState { START_SCREEN, PLAYING, MISSION_ACCOMPLISHED, MISSION_FAILED };

AppStatus gAppStatus   = RUNNING;
GameState gGameState   = START_SCREEN;

float gPrevTicks       = 0.0f;
float gTimeAccumulator = 0.0f;
float gFuel            = FUEL_MAX;
bool  gHasLeftStart    = false;

float gMovingPlatDir   = 1.0f;

Texture2D gTexBG;
Texture2D gTexTree;
Texture2D gTexAirplane;

float gAirplaneX = AIRPLANE_START_X;

Entity *gBird = nullptr;

const std::vector<int> BIRD_PINGPONG = { 0, 1, 2, 3, 4, 5, 4, 3, 2, 1 };

Entity *gCollisionPlatforms = nullptr;
Entity *gVisualTiles        = nullptr;

Vector2 gTreePositions[NUM_COLLISION_TILES];

static void drawTexCentered(const Texture2D &tex, float cx, float cy,
                             float w, float h, float angle = 0.0f)
{
    Rectangle src = { 0.0f, 0.0f, (float)tex.width, (float)tex.height };
    Rectangle dst = { cx, cy, w, h };
    Vector2   ori = { w / 2.0f, h / 2.0f };
    DrawTexturePro(tex, src, dst, ori, angle, WHITE);
}

static bool boxColliding(float ax, float ay, float aw, float ah,
                         float bx, float by, float bw, float bh)
{
    float dx = fabs(ax - bx) - ((aw + bw) / 2.0f);
    float dy = fabs(ay - by) - ((ah + bh) / 2.0f);
    return (dx < 0.0f && dy < 0.0f);
}

static void buildIsland(int idx, float cx, float topY)
{
    gCollisionPlatforms[idx].setTexture(DIRT_FP);
    gCollisionPlatforms[idx].setEntityType(PLATFORM);
    gCollisionPlatforms[idx].setScale({ PLAT_TILE_W, PLAT_TILE_H });
    gCollisionPlatforms[idx].setColliderDimensions({ PLAT_TILE_W, PLAT_TILE_H });
    gCollisionPlatforms[idx].setPosition({ cx, topY });

    int vi = idx * 2;
    gVisualTiles[vi].setTexture(DIRT_FP);
    gVisualTiles[vi].setEntityType(NONE_TYPE);
    gVisualTiles[vi].setScale({ PLAT_TILE_W * 0.70f, PLAT_TILE_H });
    gVisualTiles[vi].setColliderDimensions({ 1.0f, 1.0f });
    gVisualTiles[vi].setPosition({ cx, topY + PLAT_TILE_H });

    gVisualTiles[vi + 1].setTexture(DIRT_FP);
    gVisualTiles[vi + 1].setEntityType(NONE_TYPE);
    gVisualTiles[vi + 1].setScale({ PLAT_TILE_W * 0.40f, PLAT_TILE_H });
    gVisualTiles[vi + 1].setColliderDimensions({ 1.0f, 1.0f });
    gVisualTiles[vi + 1].setPosition({ cx, topY + PLAT_TILE_H * 2.0f });

    float treeH = 130.0f;
    gTreePositions[idx] = { cx, topY - treeH / 2.0f };
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lunar Lander: Cardinal's Descent");
    SetTargetFPS(FPS);

    gTexBG       = LoadTexture(BG_FP);
    gTexTree     = LoadTexture(TREE_FP);
    gTexAirplane = LoadTexture(AIRPLANE_FP);

    std::map<Direction, std::vector<int>> birdAtlas = {
        { RIGHT, { 0, 1, 2, 3, 4, 5 } },
        { LEFT,  { 0, 1, 2, 3, 4, 5 } },
        { UP,    { 0, 1, 2, 3, 4, 5 } },
        { DOWN,  { 0, 1, 2, 3, 4, 5 } },
    };

    gBird = new Entity(
        { ORIGIN.x, BIRD_SPAWN_Y },
        { 90.0f, 90.0f },
        BIRD_FP,
        ATLAS,
        { 1, 6 },
        birdAtlas,
        PLAYER
    );

    gBird->setPingPongFrames(BIRD_PINGPONG);
    gBird->setFrameSpeed(10);
    gBird->setColliderDimensions({ 60.0f, 60.0f });
    gBird->setAcceleration({ 0.0f, GRAVITY });

    gCollisionPlatforms = new Entity[NUM_COLLISION_TILES];
    gVisualTiles        = new Entity[NUM_VISUAL_TILES];

    buildIsland(0, SCREEN_WIDTH * 0.18f, SCREEN_HEIGHT * 0.78f);
    buildIsland(1, SCREEN_WIDTH * 0.50f, SCREEN_HEIGHT * 0.72f);
    buildIsland(2, SCREEN_WIDTH * 0.82f, SCREEN_HEIGHT * 0.78f);

    gCollisionPlatforms[3].setTexture(DIRT_FP);
    gCollisionPlatforms[3].setEntityType(MOVING_PLATFORM);
    gCollisionPlatforms[3].setScale({ PLAT_TILE_W, PLAT_TILE_H });
    gCollisionPlatforms[3].setColliderDimensions({ PLAT_TILE_W, PLAT_TILE_H });
    gCollisionPlatforms[3].setPosition({ ORIGIN.x, SCREEN_HEIGHT * 0.85f });

    gVisualTiles[6].setTexture(DIRT_FP);
    gVisualTiles[6].setEntityType(NONE_TYPE);
    gVisualTiles[6].setScale({ PLAT_TILE_W * 0.70f, PLAT_TILE_H });
    gVisualTiles[6].setColliderDimensions({ 1.0f, 1.0f });
    gVisualTiles[6].setPosition({ ORIGIN.x, SCREEN_HEIGHT * 0.85f + PLAT_TILE_H });

    gVisualTiles[7].setTexture(DIRT_FP);
    gVisualTiles[7].setEntityType(NONE_TYPE);
    gVisualTiles[7].setScale({ PLAT_TILE_W * 0.40f, PLAT_TILE_H });
    gVisualTiles[7].setColliderDimensions({ 1.0f, 1.0f });
    gVisualTiles[7].setPosition({ ORIGIN.x, SCREEN_HEIGHT * 0.85f + PLAT_TILE_H * 2.0f });
    float treeH = 130.0f;
    gTreePositions[3] = { ORIGIN.x, SCREEN_HEIGHT * 0.85f - treeH / 2.0f };

    gAirplaneX = AIRPLANE_START_X;
}

void processInput()
{
    if (IsKeyPressed(KEY_Q) || WindowShouldClose())
        gAppStatus = TERMINATED;

    if (gGameState == START_SCREEN)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            gGameState    = PLAYING;
            gHasLeftStart = false;
        }
        return;
    }

    if (gGameState != PLAYING) return;

    Vector2 accel = { 0.0f, GRAVITY };

    if (IsKeyDown(KEY_LEFT) && gFuel > 0.0f)
    {
        accel.x = -HORIZ_ACCEL;
        gFuel  -= FUEL_PER_KEY;
    }
    if (IsKeyDown(KEY_RIGHT) && gFuel > 0.0f)
    {
        accel.x = HORIZ_ACCEL;
        gFuel  -= FUEL_PER_KEY;
    }
    if (IsKeyDown(KEY_UP) && gFuel > 0.0f)
    {
        accel.y = -120.0f;
        gFuel  -= FUEL_PER_KEY;
    }

    if (gFuel < 0.0f) gFuel = 0.0f;

    gBird->setAcceleration(accel);
}

void update()
{
    if (gGameState != PLAYING) return;

    float ticks     = (float)GetTime();
    float deltaTime = ticks - gPrevTicks;
    gPrevTicks      = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        Vector2 vel = gBird->getVelocity();
        vel.x *= ACCEL_DRAG;
        if (vel.y > MAX_FALL_SPEED) vel.y = MAX_FALL_SPEED;
        gBird->setVelocity(vel);

        Vector2 mpPos = gCollisionPlatforms[3].getPosition();
        mpPos.x += MOVING_PLAT_SPEED * gMovingPlatDir * FIXED_TIMESTEP;
        float mpHalfW = gCollisionPlatforms[3].getScale().x / 2.0f;
        if (mpPos.x + mpHalfW >= SCREEN_WIDTH) gMovingPlatDir = -1.0f;
        if (mpPos.x - mpHalfW <= 0.0f)         gMovingPlatDir =  1.0f;
        gCollisionPlatforms[3].setPosition(mpPos);

        float treeH = 130.0f;
        gVisualTiles[6].setPosition({ mpPos.x, mpPos.y + PLAT_TILE_H });
        gVisualTiles[7].setPosition({ mpPos.x, mpPos.y + PLAT_TILE_H * 2.0f });
        gTreePositions[3] = { mpPos.x, mpPos.y - treeH / 2.0f };

        gAirplaneX += AIRPLANE_SPEED * FIXED_TIMESTEP;
        if (gAirplaneX - AIRPLANE_W / 2.0f > SCREEN_WIDTH)
            gAirplaneX = AIRPLANE_START_X;

        gBird->update(FIXED_TIMESTEP, gCollisionPlatforms, NUM_COLLISION_TILES);

        for (int i = 0; i < NUM_VISUAL_TILES; i++)
            gVisualTiles[i].update(FIXED_TIMESTEP, nullptr, 0);
        for (int i = 0; i < NUM_COLLISION_TILES; i++)
            gCollisionPlatforms[i].update(FIXED_TIMESTEP, nullptr, 0);

        deltaTime -= FIXED_TIMESTEP;
    }

    gTimeAccumulator = deltaTime;

    if (!gHasLeftStart)
    {
        if (gBird->getPosition().y >= BIRD_SPAWN_Y + SAFE_FALL_DIST)
            gHasLeftStart = true;
        else
            return;
    }

    {
        Vector2 bPos = gBird->getPosition();
        Vector2 bCol = gBird->getColliderDimensions();

        if (boxColliding(bPos.x, bPos.y, bCol.x, bCol.y,
                         gAirplaneX, AIRPLANE_Y,
                         AIRPLANE_W * 0.75f, AIRPLANE_H * 0.60f))
        {
            gGameState = MISSION_FAILED;
            return;
        }
    }

    if (gBird->isCollidingBottom())
    {
        Vector2 birdPos = gBird->getPosition();

        bool landedOnIsland = false;
        for (int i = 0; i < NUM_COLLISION_TILES; i++)
        {
            Vector2 cp = gCollisionPlatforms[i].getPosition();
            float   hw = gCollisionPlatforms[i].getScale().x / 2.0f;
            if (birdPos.x >= cp.x - hw && birdPos.x <= cp.x + hw)
            {
                landedOnIsland = true;
                break;
            }
        }

        gGameState = landedOnIsland ? MISSION_ACCOMPLISHED : MISSION_FAILED;
        return;
    }

    if (gBird->getPosition().y > SCREEN_HEIGHT + 100.0f ||
        gBird->getPosition().y < -200.0f                ||
        gBird->getPosition().x < -200.0f                ||
        gBird->getPosition().x > SCREEN_WIDTH  + 200.0f)
    {
        gGameState = MISSION_FAILED;
    }
}

void render()
{
    BeginDrawing();

    // Background
    {
        Rectangle src = { 0.0f, 0.0f, (float)gTexBG.width, (float)gTexBG.height };
        Rectangle dst = { 0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
        DrawTexturePro(gTexBG, src, dst, { 0.0f, 0.0f }, 0.0f, WHITE);
    }

    // Visual lower tiers (all islands including moving)
    for (int i = 0; i < NUM_VISUAL_TILES; i++)
        gVisualTiles[i].render();

    // Top collision tiles (all islands including moving)
    for (int i = 0; i < NUM_COLLISION_TILES; i++)
        gCollisionPlatforms[i].render();

    // Trees for all 4 islands
    float treeW = 110.0f, treeH = 130.0f;
    for (int i = 0; i < NUM_COLLISION_TILES; i++)
        drawTexCentered(gTexTree, gTreePositions[i].x, gTreePositions[i].y, treeW, treeH);

    // Airplane
    if (gGameState == PLAYING)
        drawTexCentered(gTexAirplane, gAirplaneX, AIRPLANE_Y, AIRPLANE_W, AIRPLANE_H, 90.0f);

    // Bird
    gBird->render();

    // Fuel UI
    if (gGameState == PLAYING || gGameState == MISSION_ACCOMPLISHED ||
        gGameState == MISSION_FAILED)
    {
        DrawText("FUEL", SCREEN_WIDTH - 130, 18, 22, WHITE);
        DrawRectangleLines(SCREEN_WIDTH - 130, 44, 110, 22, WHITE);

        float fuelRatio = gFuel / FUEL_MAX;
        int   barFill   = (int)(106.0f * fuelRatio);

        Color barColor;
        if      (fuelRatio > 0.5f)  barColor = GREEN;
        else if (fuelRatio > 0.25f) barColor = YELLOW;
        else                         barColor = RED;

        DrawRectangle(SCREEN_WIDTH - 128, 46, barFill, 18, barColor);
    }

    // Start screen
    if (gGameState == START_SCREEN)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 0, 0, 0, 160 });

        const char *title = "CARDINAL'S DESCENT";
        DrawText(title,
                 SCREEN_WIDTH / 2 - MeasureText(title, 56) / 2,
                 SCREEN_HEIGHT / 2 - 130, 56, { 255, 80, 80, 255 });

        const char *sub = "Land on any island to win!";
        DrawText(sub,
                 SCREEN_WIDTH / 2 - MeasureText(sub, 26) / 2,
                 SCREEN_HEIGHT / 2 - 48, 26, WHITE);

        const char *controls = "LEFT / RIGHT arrows: drift    UP arrow: thrust";
        DrawText(controls,
                 SCREEN_WIDTH / 2 - MeasureText(controls, 20) / 2,
                 SCREEN_HEIGHT / 2 + 2, 20, LIGHTGRAY);

        const char *warn = "Avoid the airplane!";
        DrawText(warn,
                 SCREEN_WIDTH / 2 - MeasureText(warn, 20) / 2,
                 SCREEN_HEIGHT / 2 + 36, 20, { 255, 200, 50, 255 });

        const char *prompt = "Press ENTER to begin";
        DrawText(prompt,
                 SCREEN_WIDTH / 2 - MeasureText(prompt, 30) / 2,
                 SCREEN_HEIGHT / 2 + 100, 30, { 100, 255, 100, 255 });
    }

    // Mission Accomplished
    if (gGameState == MISSION_ACCOMPLISHED)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 0, 0, 0, 160 });

        const char *msg = "MISSION ACCOMPLISHED";
        DrawText(msg,
                 SCREEN_WIDTH / 2 - MeasureText(msg, 60) / 2,
                 SCREEN_HEIGHT / 2 - 60, 60, GREEN);

        DrawText("The cardinal found its perch!",
                 SCREEN_WIDTH / 2 - MeasureText("The cardinal found its perch!", 28) / 2,
                 SCREEN_HEIGHT / 2 + 20, 28, WHITE);

        DrawText("Press Q to quit",
                 SCREEN_WIDTH / 2 - MeasureText("Press Q to quit", 24) / 2,
                 SCREEN_HEIGHT / 2 + 80, 24, LIGHTGRAY);
    }

    // Mission Failed
    if (gGameState == MISSION_FAILED)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 0, 0, 0, 160 });

        const char *msg = "MISSION FAILED";
        DrawText(msg,
                 SCREEN_WIDTH / 2 - MeasureText(msg, 60) / 2,
                 SCREEN_HEIGHT / 2 - 60, 60, RED);

        DrawText("The cardinal crashed!",
                 SCREEN_WIDTH / 2 - MeasureText("The cardinal crashed!", 28) / 2,
                 SCREEN_HEIGHT / 2 + 20, 28, WHITE);

        DrawText("Press Q to quit",
                 SCREEN_WIDTH / 2 - MeasureText("Press Q to quit", 24) / 2,
                 SCREEN_HEIGHT / 2 + 80, 24, LIGHTGRAY);
    }

    EndDrawing();
}

void shutdown()
{
    delete gBird;
    delete[] gCollisionPlatforms;
    delete[] gVisualTiles;

    UnloadTexture(gTexBG);
    UnloadTexture(gTexTree);
    UnloadTexture(gTexAirplane);

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