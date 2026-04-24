#include "CS3113/dance_gamescreen.h"
#include "CS3113/dance_menu.h"

constexpr int   SCREEN_WIDTH   = 1000;
constexpr int   SCREEN_HEIGHT  = 600;
constexpr int   FPS            = 60;
constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };

constexpr int NUMBER_OF_SCENES = 3;

AppStatus   gAppStatus       = RUNNING;
float       gPreviousTicks   = 0.0f;
float       gTimeAccumulator = 0.0f;

DanceScene *gCurrentScene = nullptr;
DanceScene *gScenes[NUMBER_OF_SCENES];

DanceMenu       *gMenu       = nullptr;
DanceGameScreen *gEasyScreen = nullptr;
DanceGameScreen *gHardScreen = nullptr;

void switchToScene(int id);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(int id)
{
    int scoreCarry = (gCurrentScene != nullptr)
                     ? gCurrentScene->getState().score
                     : 0;

    gCurrentScene = gScenes[id];
    gCurrentScene->initialise();

    if (id == 1)
    {
        static_cast<DanceGameScreen*>(gCurrentScene)->setDifficulty(DIFFICULTY_EASY);
        static_cast<DanceGameScreen*>(gCurrentScene)->setScore(scoreCarry);
    }
    else if (id == 2)
    {
        static_cast<DanceGameScreen*>(gCurrentScene)->setDifficulty(DIFFICULTY_HARD);
        static_cast<DanceGameScreen*>(gCurrentScene)->setScore(scoreCarry);
    }
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dance Rush");
    InitAudioDevice();
    SetTargetFPS(FPS);

    gMenu       = new DanceMenu      (ORIGIN, "#7DD8F5");
    gEasyScreen = new DanceGameScreen(ORIGIN, "#7DD8F5");
    gHardScreen = new DanceGameScreen(ORIGIN, "#7DD8F5");

    gEasyScreen->setDifficulty(DIFFICULTY_EASY);
    gHardScreen->setDifficulty(DIFFICULTY_HARD);

    gScenes[0] = gMenu;
    gScenes[1] = gEasyScreen;
    gScenes[2] = gHardScreen;

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
}

void update()
{
    float ticks     = (float)GetTime();
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
    }

    gTimeAccumulator = deltaTime;
}

void render()
{
    BeginDrawing();
    gCurrentScene->render();
    EndDrawing();
}

void shutdown()
{
    delete gMenu;
    delete gEasyScreen;
    delete gHardScreen;

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

        if (nextID == -1)
            gAppStatus = TERMINATED;
        else if (nextID == -2)
            switchToScene(0);
        else if (nextID > 0)
            switchToScene(nextID);

        render();
    }

    shutdown();
    return 0;
}
