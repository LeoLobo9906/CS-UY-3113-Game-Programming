#include "menu.h"

Menu::Menu()                                        : Scene{ {0.0f}, nullptr   } {}
Menu::Menu(Vector2 origin, const char *bgHexCode)   : Scene{ origin, bgHexCode } {}
Menu::~Menu() { shutdown(); }

void Menu::initialise()
{
    mGameState.nextSceneID = 0;
    mGameState.lives       = 3;
    mGameState.mario       = nullptr;
    mGameState.enemies     = nullptr;
    mGameState.coins       = nullptr;
    mGameState.map         = nullptr;
}

void Menu::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER)) mGameState.nextSceneID = 1;
}

void Menu::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    const char *title = "SUPER MARIO BROS";
    DrawText(title,
             (int)mOrigin.x - MeasureText(title, 60) / 2,
             (int)mOrigin.y - 120, 60, YELLOW);

    const char *sub = "Collect all coins to advance!";
    DrawText(sub,
             (int)mOrigin.x - MeasureText(sub, 24) / 2,
             (int)mOrigin.y - 30, 24, WHITE);

    const char *ctrl = "A / D : Move     W : Jump";
    DrawText(ctrl,
             (int)mOrigin.x - MeasureText(ctrl, 20) / 2,
             (int)mOrigin.y + 20, 20, LIGHTGRAY);

    const char *prompt = "Press ENTER to Start";
    DrawText(prompt,
             (int)mOrigin.x - MeasureText(prompt, 32) / 2,
             (int)mOrigin.y + 90, 32, GREEN);

    const char *lives = "You start with 3 lives. Don't touch enemies!";
    DrawText(lives,
             (int)mOrigin.x - MeasureText(lives, 18) / 2,
             (int)mOrigin.y + 155, 18, RED);
}

void Menu::shutdown() {}