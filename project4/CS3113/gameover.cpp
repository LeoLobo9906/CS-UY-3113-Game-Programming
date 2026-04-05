#include "gameover.h"

GameOver::GameOver()                                      : Scene{ {0.0f}, nullptr   } {}
GameOver::GameOver(Vector2 origin, const char *bgHexCode) : Scene{ origin, bgHexCode } {}
GameOver::~GameOver() { shutdown(); }

void GameOver::initialise()
{
    mGameState.nextSceneID = 0;
    mGameState.mario       = nullptr;
    mGameState.enemies     = nullptr;
    mGameState.coins       = nullptr;
    mGameState.map         = nullptr;
}

void GameOver::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER)) mGameState.nextSceneID = 0;
}

void GameOver::render()
{
    ClearBackground(BLACK);

    const char *msg = "GAME OVER";
    DrawText(msg,
             (int)mOrigin.x - MeasureText(msg, 72) / 2,
             (int)mOrigin.y - 100, 72, RED);

    const char *sub = "You ran out of lives!";
    DrawText(sub,
             (int)mOrigin.x - MeasureText(sub, 28) / 2,
             (int)mOrigin.y + 10, 28, WHITE);

    const char *prompt = "Press ENTER to return to Menu";
    DrawText(prompt,
             (int)mOrigin.x - MeasureText(prompt, 24) / 2,
             (int)mOrigin.y + 80, 24, YELLOW);
}

void GameOver::shutdown() {}