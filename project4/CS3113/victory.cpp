#include "victory.h"

Victory::Victory()                                        : Scene{ {0.0f}, nullptr   } {}
Victory::Victory(Vector2 origin, const char *bgHexCode)   : Scene{ origin, bgHexCode } {}
Victory::~Victory() { shutdown(); }

void Victory::initialise()
{
    mGameState.nextSceneID = 0;
    mGameState.mario       = nullptr;
    mGameState.enemies     = nullptr;
    mGameState.coins       = nullptr;
    mGameState.map         = nullptr;
}

void Victory::update(float deltaTime)
{
    if (IsKeyPressed(KEY_E)) mGameState.nextSceneID = 0;
}

void Victory::render()
{
    ClearBackground(ColorFromHex("#5C94FC"));

    const char *msg = "YOU WIN!";
    DrawText(msg,
             (int)mOrigin.x - MeasureText(msg, 80) / 2,
             (int)mOrigin.y - 120, 80, YELLOW);

    const char *sub = "Thank you Mario!";
    DrawText(sub,
             (int)mOrigin.x - MeasureText(sub, 36) / 2,
             (int)mOrigin.y - 10, 36, WHITE);

    const char *sub2 = "All coins collected across all 3 worlds!";
    DrawText(sub2,
             (int)mOrigin.x - MeasureText(sub2, 22) / 2,
             (int)mOrigin.y + 50, 22, LIGHTGRAY);

    const char *prompt = "Press E to return to Menu";
    DrawText(prompt,
             (int)mOrigin.x - MeasureText(prompt, 24) / 2,
             (int)mOrigin.y + 120, 24, GREEN);
}

void Victory::shutdown() {}