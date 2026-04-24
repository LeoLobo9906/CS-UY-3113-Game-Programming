#include "dance_menu.h"
#include <math.h>
#include <algorithm>

DanceMenu::DanceMenu()
    : DanceScene{ {0.0f}, nullptr } {}

DanceMenu::DanceMenu(Vector2 origin, const char *bgHexCode)
    : DanceScene{ origin, bgHexCode } {}

DanceMenu::~DanceMenu() { shutdown(); }

void DanceMenu::initialise()
{
    mGameState.nextSceneID = 0;
    mGameState.score       = 0;

    mBgTile = LoadTexture("assets/background.png");
    mBoxTex = LoadTexture("assets/background_box.png");
    SetTextureFilter(mBgTile, TEXTURE_FILTER_POINT);
    SetTextureFilter(mBoxTex, TEXTURE_FILTER_POINT);

    Image img = LoadImage("assets/dancer.png");
    ImageColorReplace(&img, BLACK, BLANK);
    mDancer.texture = LoadTextureFromImage(img);
    UnloadImage(img);
    SetTextureFilter(mDancer.texture, TEXTURE_FILTER_POINT);

    mDancerShader = LoadShader(
        "assets/shaders/menu_dancer_vertex.glsl",
        "assets/shaders/menu_dancer_fragment.glsl"
    );

    mShaderTimeLoc = GetShaderLocation(mDancerShader, "time");

    mDancer.frameCount   = 4;
    mDancer.currentFrame = 0;
    mDancer.frameTimer   = 0.0f;
    mDancer.frameSpeed   = 8.0f;
    mDancer.drawSize     = 192.0f;
    mDancer.position     = { mOrigin.x, mOrigin.y + 50.0f };

    mGameState.bgm = LoadMusicStream("assets/menu_intro.wav");
    SetMusicVolume(mGameState.bgm, 0.7f);
    PlayMusicStream(mGameState.bgm);

    mTitlePulse = 0.0f;
    mSelectedOption = 0;
}

void DanceMenu::update(float deltaTime)
{
    float t = (float)GetTime();
    if (mDancerShader.id != 0 && mShaderTimeLoc != -1)
        SetShaderValue(mDancerShader, mShaderTimeLoc, &t, SHADER_UNIFORM_FLOAT);

    UpdateMusicStream(mGameState.bgm);
    updateDancer(deltaTime);

    mTitlePulse += deltaTime * 2.2f;
    if (mTitlePulse > 6.28318f) mTitlePulse -= 6.28318f;

    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        mSelectedOption = 0;

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        mSelectedOption = 1;

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        StopMusicStream(mGameState.bgm);

        if (mSelectedOption == 0)
            mGameState.nextSceneID = 1;
        else
            mGameState.nextSceneID = 2;
    }
}

void DanceMenu::render()
{
    drawTiledBackground(GetScreenWidth(), GetScreenHeight());
    DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(),
                           {0,0,0,40}, {0,0,0,100});

    float bob     = sinf(mTitlePulse) * 5.0f;
    int   titleSz = 72;
    const char *title = "DANCE RUSH";
    int titleW = MeasureText(title, titleSz);

    DrawText(title, (int)mOrigin.x - titleW/2 + 4,
             (int)(mOrigin.y - 270.0f + bob) + 4, titleSz, {0,0,0,100});
    DrawText(title, (int)mOrigin.x - titleW/2,
             (int)(mOrigin.y - 270.0f + bob), titleSz, WHITE);

    const char *sub = "RHYTHM  |  BEAT  |  FLOW";
    int subW = MeasureText(sub, 22);
    DrawText(sub, (int)mOrigin.x - subW/2,
             (int)(mOrigin.y - 190.0f), 22, {200,240,255,220});

    if (mBoxTex.id != 0)
    {
        float bw = (float)mBoxTex.width * 3.0f;
        float bh = (float)mBoxTex.height * 3.0f;

        DrawTexturePro(
            mBoxTex,
            {0,0,(float)mBoxTex.width,(float)mBoxTex.height},
            {mDancer.position.x - bw/2, mDancer.position.y - bh/2, bw, bh},
            {0,0}, 0.0f, WHITE
        );
    }

    static const int FX[4] = {  32, 100, 200, 300 };
    static const int FY[4] = {   0,   8,   8,   0 };
    static const int FW[4] = {  68, 100, 100,  59 };
    static const int FH[4] = {  96,  87,  86,  94 };

    int f = mDancer.currentFrame;

    Rectangle src = {
        (float)FX[f],
        (float)FY[f],
        (float)FW[f],
        (float)FH[f]
    };

    float scale = mDancer.drawSize / (float)std::max(FW[f], FH[f]);
    float dw = FW[f] * scale;
    float dh = FH[f] * scale;

    Rectangle dst = {
        mDancer.position.x - dw / 2.0f,
        mDancer.position.y - dh / 2.0f,
        dw,
        dh
    };

    if (mDancerShader.id != 0) BeginShaderMode(mDancerShader);
    DrawTexturePro(mDancer.texture, src, dst, {0, 0}, 0.0f, WHITE);
    if (mDancerShader.id != 0) EndShaderMode();

    const char *easy = "EASY";
    const char *hard = "HARD";

    int easySz = (mSelectedOption == 0) ? 34 : 28;
    int hardSz = (mSelectedOption == 1) ? 34 : 28;

    Color selected = {255, 220, 80, 255};
    Color normal   = {255, 255, 255, 190};

    int easyW = MeasureText(easy, easySz);
    int hardW = MeasureText(hard, hardSz);

    DrawText(easy,
             (int)mOrigin.x - easyW/2,
             (int)(mOrigin.y + 180.0f),
             easySz,
             (mSelectedOption == 0) ? selected : normal);

    DrawText(hard,
             (int)mOrigin.x - hardW/2,
             (int)(mOrigin.y + 225.0f),
             hardSz,
             (mSelectedOption == 1) ? selected : normal);

    const char *arrow = ">";
    if (mSelectedOption == 0)
        DrawText(arrow, (int)mOrigin.x - easyW/2 - 35, (int)(mOrigin.y + 184.0f), 28, selected);
    else
        DrawText(arrow, (int)mOrigin.x - hardW/2 - 35, (int)(mOrigin.y + 229.0f), 28, selected);

    unsigned char blinkA = (sinf(mTitlePulse * 1.8f) > 0.0f) ? 255 : 0;

    const char *prompt = "PRESS ENTER TO START";
    int promptW = MeasureText(prompt, 22);
    DrawText(prompt, (int)mOrigin.x - promptW/2,
             (int)(mOrigin.y + 300.0f), 22, {255,255,255,blinkA});

    float guideX = mOrigin.x + 210.0f;
    float guideY = mOrigin.y + 120.0f;

    DrawText("CONTROLS", (int)guideX, (int)guideY, 20, {255,220,80,255});
    DrawText("ARROWS / WASD", (int)guideX, (int)(guideY + 35.0f), 16, {220,240,255,220});
    DrawText("HIT NOTES", (int)guideX, (int)(guideY + 58.0f), 16, {220,240,255,220});
    DrawText("UP / DOWN", (int)guideX, (int)(guideY + 95.0f), 16, {220,240,255,220});
    DrawText("SELECT LEVEL", (int)guideX, (int)(guideY + 118.0f), 16, {220,240,255,220});
    DrawText("ENTER = START", (int)guideX, (int)(guideY + 155.0f), 16, {220,240,255,220});
    DrawText("ESC = MENU", (int)guideX, (int)(guideY + 178.0f), 16, {220,240,255,180});
    DrawText("Q = QUIT", (int)guideX, (int)(guideY + 201.0f), 16, {220,240,255,180});
}

void DanceMenu::shutdown()
{
    if (mDancerShader.id != 0) UnloadShader(mDancerShader);
    if (mDancer.texture.id != 0) UnloadTexture(mDancer.texture);
    if (mBoxTex.id         != 0) UnloadTexture(mBoxTex);
    if (mBgTile.id         != 0) UnloadTexture(mBgTile);
    UnloadMusicStream(mGameState.bgm);

    mDancerShader.id = 0;
    mDancer.texture.id = 0;
    mBoxTex.id = 0;
    mBgTile.id = 0;
}