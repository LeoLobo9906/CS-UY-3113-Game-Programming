#include "CS3113/cs3113.h"
#include <math.h>

enum Member {
    MURDOC,
    TWO_D,
    RUSSEL,
    NOODLE
};

// Global Constants
constexpr int   SCREEN_WIDTH  = 1600 / 2,
                SCREEN_HEIGHT = 900 / 2,
                FPS           = 60,
                SIZE          = 200,
                FRAME_LIMIT   = 100;
constexpr float MAX_AMP       = 10.0f;

constexpr char    BG_COLOUR[] = "#000000";
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr Vector2 BASE_SIZE   = { (float) SIZE, (float) SIZE };

// Image owned by Gorillaz @see https://gorillaz.com/
constexpr char ALBUM_COVER_FP[] = "assets/demon_days.png";

// Global Variables
AppStatus gAppStatus     = RUNNING;
float     gScaleFactor   = SIZE,
          gAngle         = 0.0f,
          gPulseTime     = 0.0f;
Vector2   gPosition      = ORIGIN;
Vector2   gScale         = BASE_SIZE;
Texture2D gTeardropTexture;

float gPreviousTicks = 0.0f;
int gFrameCount = 0;

Member gMember;
float start_width;
float start_height;
float cut_width;
float cut_height;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Function Definitions
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures");

    gTeardropTexture = LoadTexture(ALBUM_COVER_FP);

    SetTargetFPS(FPS);
}

void processInput()
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    /**
     * @todo Calculate delta time
     */
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    /**
     * @todo Apply delta time to the time-dependent logic
     */
    gPulseTime += 1.0f * deltaTime;

    gScale = {
        BASE_SIZE.x + MAX_AMP * cos(gPulseTime),
        BASE_SIZE.y + MAX_AMP * cos(gPulseTime)
    };

    /**
     * @todo Switch member every 100 fames
     */
    gFrameCount += 1;
    if (gFrameCount <= 100) {
        gMember = MURDOC;
        start_width = 0.0f;
        start_height = 0.0f;
    }
    else if (gFrameCount <= 200) {
        gMember = TWO_D;
        start_width = static_cast<float>(gTeardropTexture.width) / 2.0f;
        start_height = 0.0f;
    }
    else if (gFrameCount <= 300) {
        gMember = RUSSEL;
        start_width = static_cast<float>(gTeardropTexture.width) / 2.0f;
        start_height = static_cast<float>(gTeardropTexture.height) / 2.0f;
    }
    else if (gFrameCount <= 400) {
        gMember = NOODLE;
        start_width = 0.0f;
        start_height = static_cast<float>(gTeardropTexture.height) / 2.0f;
    }
    else {
        gFrameCount = 0;
        gMember = MURDOC;
    }


}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    /**
     * @todo Design your UV coordinates (i.e. textureArea) so that only one
     * member is being rendered onto the screen.
     */

    Rectangle textureArea = {
        // top-left corner
        start_width, start_height,

        // bottom-right corner (of texture)
        static_cast<float>(gTeardropTexture.width) / 2.0f,
        static_cast<float>(gTeardropTexture.height) / 2.0f
    };

    // Destination rectangle – centred on gPosition
    Rectangle destinationArea = {
        gPosition.x,
        gPosition.y,
        static_cast<float>(gScale.x),
        static_cast<float>(gScale.y)
    };

    // Origin inside the source texture (centre of the texture)
    Vector2 objectOrigin = {
        static_cast<float>(gScale.x) / 2.0f,
        static_cast<float>(gScale.y) / 2.0f
    };

    // Render the texture on screen
    DrawTexturePro(
        gTeardropTexture,
        textureArea,
        destinationArea,
        objectOrigin,
        gAngle,
        WHITE
    );

    EndDrawing();
}

void shutdown() { CloseWindow(); }

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