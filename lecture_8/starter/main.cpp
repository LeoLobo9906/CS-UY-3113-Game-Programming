#include "CS3113/Entity.h"

struct GameState
{
    Entity *xochitl;
    Map *map;

    Music bgm;
    Sound jumpSound;
};

// Global Constants
constexpr int SCREEN_WIDTH  = 1000,
              SCREEN_HEIGHT = 600,
              FPS           = 120;

constexpr char    BG_COLOUR[]      = "#C0897E";
constexpr Vector2 ORIGIN           = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
                  ATLAS_DIMENSIONS = { 6, 8 };


// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f;

GameState gState;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maps");
    InitAudioDevice();

    gState.bgm = LoadMusicStream("assets/game/04 - Silent Forest.wav");
    SetMusicVolume(gState.bgm, 0.33f);
    PlayMusicStream(gState.bgm);

    gState.jumpSound = LoadSound("assets/game/Dirt Jump.wav");

    gState.map = new  Map(
        14, 8,
        LEVEL_DATA,
        "assets/game/tileset.png",
        TILE_DIMENSION,
        4, 1,
        ORIGIN
    );


    /*
        ----------- PROTAGONIST -----------
    */
    std::map<Direction, std::vector<int>> xochitlAnimationAtlas = {
        {DOWN,  {  0,  1,  2,  3,  4,  5,  6,  7 }},
        {LEFT,  {  8,  9, 10, 11, 12, 13, 14, 15 }},
        {UP,    { 24, 25, 26, 27, 28, 29, 30, 31 }},
        {RIGHT, { 40, 41, 42, 43, 44, 45, 46, 47 }},
    };

    float sizeRatio = 48.0f / 64.0f;

    // Assets from @see https://sscary.itch.io/the-adventurer-female
    gState.xochitl = new Entity(
        {ORIGIN.x - 300.0f, ORIGIN.y - 200.0f}, // position
        {250.0f * sizeRatio, 250.0f},            // scale
        "assets/game/walk.png",                  // texture file address
        ATLAS,                                   // single image or atlas?
        ATLAS_DIMENSIONS,                        // atlas dimensions
        xochitlAnimationAtlas,                   // actual atlas
        PLAYER                                   // entity type
    );

    gState.xochitl->setJumpingPower(450.0f);
    gState.xochitl->setColliderDimensions({
        gState.xochitl->getScale().x / 3.0f,
        gState.xochitl->getScale().y / 3.0f
    });
    gState.xochitl->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});


void processInput()
{
    gState.xochitl->resetMovement();

    if      (IsKeyDown(KEY_A)) gState.xochitl->moveLeft();
    else if (IsKeyDown(KEY_D)) gState.xochitl->moveRight();

    if (IsKeyPressed(KEY_W) && gState.xochitl->isCollidingBottom())
    {
        gState.xochitl->jump();
        PlaySound(gState.jumpSound);
    }

    // to avoid faster diagonal speed
    if (GetLength(gState.xochitl->getMovement()) > 1.0f)
        gState.xochitl->normaliseMovement();

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    // Delta time
    float ticks    = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    // Fixed timestep
    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        UpdateMusicStream(gState.bgm);

        gState.xochitl->update(
            FIXED_TIMESTEP,  // delta time / fixed timestep
            nullptr,         // player
            nullptr,         // map
        );


        deltaTime -= FIXED_TIMESTEP;
    }

    if (gState.xochitl->getPosition().y > END_GAME_THRESHOLD)
        gAppStatus = TERMINATED;
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    gState.xochitl->render();


    EndDrawing();
}

void shutdown()
{
    delete   gState.xochitl;
    delete gState.map;

    UnloadMusicStream(gState.bgm);
    UnloadSound(gState.jumpSound);

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
        render();
    }

    shutdown();

    return 0;
}
