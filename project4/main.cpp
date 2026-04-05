/**
* Author: Leonardo Villalobos
* Assignment: Rise of the AI
* Date due: 2026-04-04, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/victory.h"
#include "CS3113/gameover.h"
#include "CS3113/levelc.h"
#include "CS3113/levelb.h"
#include "CS3113/levela.h"
#include "CS3113/menu.h"

constexpr int   SCREEN_WIDTH   = 1000;
constexpr int   SCREEN_HEIGHT  = 600;
constexpr int   FPS            = 60;
constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };

constexpr int NUMBER_OF_SCENES = 6;

AppStatus gAppStatus     = RUNNING;
float     gPreviousTicks = 0.0f;
float     gTimeAccumulator = 0.0f;

Camera2D gCamera = { 0 };

Scene   *gCurrentScene = nullptr;
Scene   *gScenes[NUMBER_OF_SCENES];

Menu    *gMenu     = nullptr;
LevelA  *gLevelA   = nullptr;
LevelB  *gLevelB   = nullptr;
LevelC  *gLevelC   = nullptr;
GameOver *gGameOver = nullptr;
Victory  *gVictory  = nullptr;

void switchToScene(int id);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(int id)
{
    int livesCarry = (gCurrentScene != nullptr)
                     ? gCurrentScene->getState().lives
                     : 3;

    gCurrentScene = gScenes[id];
    gCurrentScene->initialise();

    if      (id == 1) static_cast<LevelA*>(gCurrentScene)->setLives(livesCarry);
    else if (id == 2) static_cast<LevelB*>(gCurrentScene)->setLives(livesCarry);
    else if (id == 3) static_cast<LevelC*>(gCurrentScene)->setLives(livesCarry);

    if (id >= 1 && id <= 3)
    {
        gCamera.target = gCurrentScene->getState().mario->getPosition();
    }
    else
    {
        gCamera.target = ORIGIN;
    }
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Super Mario Bros — Rise of the AI");
    InitAudioDevice();
    SetTargetFPS(FPS);

    gMenu     = new Menu    (ORIGIN, "#5C94FC");
    gLevelA   = new LevelA  (ORIGIN, "#5C94FC");
    gLevelB   = new LevelB  (ORIGIN, "#5C94FC");
    gLevelC   = new LevelC  (ORIGIN, "#000000");
    gGameOver = new GameOver(ORIGIN, "#000000");
    gVictory  = new Victory (ORIGIN, "#5C94FC");

    gScenes[0] = gMenu;
    gScenes[1] = gLevelA;
    gScenes[2] = gLevelB;
    gScenes[3] = gLevelC;
    gScenes[4] = gGameOver;
    gScenes[5] = gVictory;

    gCamera.offset   = ORIGIN;
    gCamera.rotation = 0.0f;
    gCamera.zoom     = 1.0f;

    gCurrentScene = nullptr;
    switchToScene(0);
}

void processInput()
{
    if (IsKeyPressed(KEY_Q) || WindowShouldClose())
    {
        gAppStatus = TERMINATED;
        return;
    }

    if (IsKeyPressed(KEY_ONE))   switchToScene(1);
    if (IsKeyPressed(KEY_TWO))   switchToScene(2);
    if (IsKeyPressed(KEY_THREE)) switchToScene(3);

    GameState *state = gCurrentScene->getStatePtr();

    if (!state->mario) return;

    state->mario->resetMovement();

    if      (IsKeyDown(KEY_A)) state->mario->moveLeft();
    else if (IsKeyDown(KEY_D)) state->mario->moveRight();

    if (IsKeyPressed(KEY_W) && state->mario->isCollidingBottom())
    {
        state->mario->jump();
        PlaySound(state->jumpSound);
    }

    if (GetLength(state->mario->getMovement()) > 1.0f)
        state->mario->normaliseMovement();
}

void update()
{
    float ticks    = (float)GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;

        GameState *state = gCurrentScene->getStatePtr();
        if (state->mario)
        {
            Vector2 target = {
                state->mario->getPosition().x,
                ORIGIN.y
            };
            panCamera(&gCamera, &target);
        }
    }

    gTimeAccumulator = deltaTime;
}

void render()
{
    BeginDrawing();

    GameState *state = gCurrentScene->getStatePtr();

    if (state->mario)
    {
        BeginMode2D(gCamera);
        gCurrentScene->render();
        EndMode2D();

        DrawText("LIVES:", 10, 10, 24, WHITE);
        for (int i = 0; i < state->lives; i++)
            DrawText("<3", 100 + i * 32, 10, 22, RED);

        char coinBuf[32];
        sprintf(coinBuf, "COINS LEFT: %d", state->coinsLeft);
        DrawText(coinBuf,
                 SCREEN_WIDTH - MeasureText(coinBuf, 22) - 10,
                 10, 22, YELLOW);

        int sceneIdx = 0;
        for (int i = 0; i < NUMBER_OF_SCENES; i++)
            if (gScenes[i] == gCurrentScene) { sceneIdx = i; break; }

        const char *levelLabels[] = { "", "WORLD 1-1", "WORLD 1-2", "WORLD 1-3", "", "" };
        DrawText(levelLabels[sceneIdx],
                 SCREEN_WIDTH / 2 - MeasureText(levelLabels[sceneIdx], 22) / 2,
                 10, 22, WHITE);
    }
    else
    {
        gCurrentScene->render();
    }

    EndDrawing();
}

void shutdown()
{
    delete gMenu;
    delete gLevelA;
    delete gLevelB;
    delete gLevelC;
    delete gGameOver;
    delete gVictory;

    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();

        int nextID = gCurrentScene->getState().nextSceneID;
        if (nextID > 0)
        {
            switchToScene(nextID);
        }

        render();
    }

    shutdown();
    return 0;
}