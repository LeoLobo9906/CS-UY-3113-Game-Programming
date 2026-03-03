/**
* Author: Leonardo Villalobos
* Assignment: Pong Clone
* Date due: 02/28/2026
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/cs3113.h"
#include <math.h>

// ─── Screen / Game Constants ──────────────────────────────────────────────────
constexpr int SCREEN_WIDTH  = (int)(800 * 1.5f);
constexpr int SCREEN_HEIGHT = (int)(450 * 1.5f);
constexpr int FPS           = 60;
constexpr int WIN_SCORE     = 3;

constexpr Vector2 ORIGIN = {
    SCREEN_WIDTH  / 2.0f,
    SCREEN_HEIGHT / 2.0f
};

// ─── Paddle Constants ─────────────────────────────────────────────────────────
constexpr float PADDLE_W      = 80.0f;
constexpr float PADDLE_H      = 120.0f;
constexpr float PADDLE_SPEED  = 420.0f;
constexpr float PADDLE_MARGIN = 40.0f;

// ─── Ball Constants ───────────────────────────────────────────────────────────
constexpr float BALL_SIZE       = 60.0f;
constexpr float BALL_SPEED_INIT = 380.0f;
constexpr int   MAX_BALLS       = 3;

// ─── Asset Paths ──────────────────────────────────────────────────────────────
constexpr char BG_FP[]     = "assets/background field.png";
constexpr char SONIC_FP[]  = "assets/sonic.png";
constexpr char MARIO_FP[]  = "assets/mario.png";
constexpr char BALL_FP[]   = "assets/ball.png";
constexpr char ENDMSG_FP[] = "assets/end_message.png";

// ─── Enums ────────────────────────────────────────────────────────────────────
enum GameState   { PLAYING, GAME_OVER };
enum AIDirection { AI_UP, AI_DOWN };   // same pattern as Umbreon Bigger/Smaller

// ─── Global State ─────────────────────────────────────────────────────────────
AppStatus  gAppStatus   = RUNNING;
GameState  gGameState   = PLAYING;
float      gPrevTicks   = 0.0f;

bool       gMarioIsAI   = false;
int        gActiveBalls = 1;
int        gSonicScore  = 0;
int        gMarioScore  = 0;
int        gWinner      = 0;   // 1 = Sonic, 2 = Mario

// AI bounce direction — same idea as your Umbreon Bigger/Smaller enum
AIDirection gAIDir = AI_DOWN;

// ─── Textures ─────────────────────────────────────────────────────────────────
Texture2D gTexBG;
Texture2D gTexSonic;
Texture2D gTexMario;
Texture2D gTexBall;
Texture2D gTexEndMsg;

// ─── Paddle Data ──────────────────────────────────────────────────────────────
Vector2 gSonicPos   = { PADDLE_MARGIN + PADDLE_W / 2.0f, ORIGIN.y };
Vector2 gSonicScale = { PADDLE_W, PADDLE_H };
Vector2 gSonicMov   = { 0.0f, 0.0f };

Vector2 gMarioPos   = { SCREEN_WIDTH - PADDLE_MARGIN - PADDLE_W / 2.0f, ORIGIN.y };
Vector2 gMarioScale = { PADDLE_W, PADDLE_H };
Vector2 gMarioMov   = { 0.0f, 0.0f };

// ─── Ball Data ────────────────────────────────────────────────────────────────
struct Ball {
    Vector2 pos;
    Vector2 vel;
    Vector2 scale;
    bool    active;
};

Ball gBalls[MAX_BALLS];

// ─── Helper: draw texture centred at pos ──────────────────────────────────────
void drawTex(const Texture2D &tex, Vector2 pos, Vector2 scale, float angle = 0.0f)
{
    Rectangle src = { 0.0f, 0.0f, (float)tex.width, (float)tex.height };
    Rectangle dst = { pos.x, pos.y, scale.x, scale.y };
    Vector2   ori = { scale.x / 2.0f, scale.y / 2.0f };
    DrawTexturePro(tex, src, dst, ori, angle, WHITE);
}

// ─── Box-to-box collision (from lecture 4 / your exercise code) ───────────────
bool isColliding(const Vector2 *posA, const Vector2 *scaleA,
                 const Vector2 *posB, const Vector2 *scaleB)
{
    float dx = fabs(posA->x - posB->x) - ((scaleA->x + scaleB->x) / 2.0f);
    float dy = fabs(posA->y - posB->y) - ((scaleA->y + scaleB->y) / 2.0f);
    return (dx < 0.0f && dy < 0.0f);
}

// ─── Reset one ball to centre with a fresh velocity ───────────────────────────
void resetBall(int i)
{
    gBalls[i].scale = { BALL_SIZE, BALL_SIZE };

    // Stagger Y so multiple balls don't overlap
    gBalls[i].pos = {
        ORIGIN.x,
        ORIGIN.y + i * 70.0f - (MAX_BALLS - 1) * 35.0f
    };

    // Alternate serve direction per ball index
    float dx = (i % 2 == 0) ?  BALL_SPEED_INIT : -BALL_SPEED_INIT;
    float dy = (i % 2 == 0) ?  BALL_SPEED_INIT * 0.6f
                             : -BALL_SPEED_INIT * 0.6f;

    gBalls[i].vel    = { dx, dy };
    gBalls[i].active = (i < gActiveBalls);
}

// ─── Initialise ───────────────────────────────────────────────────────────────
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sonic vs Mario: Soccer Pong");
    SetTargetFPS(FPS);

    gTexBG     = LoadTexture(BG_FP);
    gTexSonic  = LoadTexture(SONIC_FP);
    gTexMario  = LoadTexture(MARIO_FP);
    gTexBall   = LoadTexture(BALL_FP);
    gTexEndMsg = LoadTexture(ENDMSG_FP);

    for (int i = 0; i < MAX_BALLS; i++) resetBall(i);
}

// ─── Process Input ────────────────────────────────────────────────────────────
void processInput()
{
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;

    if (gGameState == GAME_OVER) return;

    // Toggle AI for Mario
    if (IsKeyPressed(KEY_T)) gMarioIsAI = !gMarioIsAI;

    // Number keys 1-3: change active ball count
    if (IsKeyPressed(KEY_ONE))   gActiveBalls = 1;
    if (IsKeyPressed(KEY_TWO))   gActiveBalls = 2;
    if (IsKeyPressed(KEY_THREE)) gActiveBalls = 3;

    // Activate/deactivate balls to match new count
    for (int i = 0; i < MAX_BALLS; i++)
    {
        bool shouldBeActive = (i < gActiveBalls);
        if (shouldBeActive && !gBalls[i].active) resetBall(i);
        gBalls[i].active = shouldBeActive;
    }

    // Sonic — W / S
    gSonicMov = { 0.0f, 0.0f };
    if (IsKeyDown(KEY_W)) gSonicMov.y = -1.0f;
    if (IsKeyDown(KEY_S)) gSonicMov.y =  1.0f;

    // Mario — Up / Down (locked out when AI is on)
    gMarioMov = { 0.0f, 0.0f };
    if (!gMarioIsAI)
    {
        if (IsKeyDown(KEY_UP))   gMarioMov.y = -1.0f;
        if (IsKeyDown(KEY_DOWN)) gMarioMov.y =  1.0f;
    }
}

// ─── Update ───────────────────────────────────────────────────────────────────
void update()
{
    if (gGameState == GAME_OVER) return;

    float ticks     = (float)GetTime();
    float deltaTime = ticks - gPrevTicks;
    gPrevTicks      = ticks;

    // ── Sonic movement + screen clamp
    gSonicPos.y += PADDLE_SPEED * gSonicMov.y * deltaTime;
    if (gSonicPos.y - PADDLE_H / 2.0f < 0.0f)
        gSonicPos.y = PADDLE_H / 2.0f;
    if (gSonicPos.y + PADDLE_H / 2.0f > SCREEN_HEIGHT)
        gSonicPos.y = SCREEN_HEIGHT - PADDLE_H / 2.0f;

    // ── Mario AI: bounce up and down, flip at edges
    //    Same pattern as your Umbreon Bigger/Smaller direction code
    if (gMarioIsAI)
    {
        if (gAIDir == AI_DOWN) gMarioMov.y =  1.0f;
        else                   gMarioMov.y = -1.0f;

        if (gMarioPos.y + PADDLE_H / 2.0f >= SCREEN_HEIGHT) gAIDir = AI_UP;
        if (gMarioPos.y - PADDLE_H / 2.0f <= 0.0f)          gAIDir = AI_DOWN;
    }

    // ── Mario movement + screen clamp
    gMarioPos.y += PADDLE_SPEED * gMarioMov.y * deltaTime;
    if (gMarioPos.y - PADDLE_H / 2.0f < 0.0f)
        gMarioPos.y = PADDLE_H / 2.0f;
    if (gMarioPos.y + PADDLE_H / 2.0f > SCREEN_HEIGHT)
        gMarioPos.y = SCREEN_HEIGHT - PADDLE_H / 2.0f;

    // ── Update each active ball
    for (int i = 0; i < MAX_BALLS; i++)
    {
        if (!gBalls[i].active) continue;

        Ball &b = gBalls[i];

        b.pos.x += b.vel.x * deltaTime;
        b.pos.y += b.vel.y * deltaTime;

        // Top / bottom wall bounce
        if (b.pos.y - BALL_SIZE / 2.0f <= 0.0f)
        {
            b.pos.y = BALL_SIZE / 2.0f;
            b.vel.y = fabs(b.vel.y);
        }
        if (b.pos.y + BALL_SIZE / 2.0f >= SCREEN_HEIGHT)
        {
            b.pos.y = SCREEN_HEIGHT - BALL_SIZE / 2.0f;
            b.vel.y = -fabs(b.vel.y);
        }

        // Sonic paddle collision (left side)
        if (isColliding(&b.pos, &b.scale, &gSonicPos, &gSonicScale))
        {
            b.pos.x = gSonicPos.x + (gSonicScale.x + b.scale.x) / 2.0f;
            b.vel.x = fabs(b.vel.x);
        }

        // Mario paddle collision (right side)
        if (isColliding(&b.pos, &b.scale, &gMarioPos, &gMarioScale))
        {
            b.pos.x = gMarioPos.x - (gMarioScale.x + b.scale.x) / 2.0f;
            b.vel.x = -fabs(b.vel.x);
        }

        // Ball exits left → Mario scores
        if (b.pos.x + BALL_SIZE / 2.0f < 0.0f)
        {
            gMarioScore++;
            if (gMarioScore >= WIN_SCORE) { gGameState = GAME_OVER; gWinner = 2; }
            else resetBall(i);
        }

        // Ball exits right → Sonic scores
        if (b.pos.x - BALL_SIZE / 2.0f > SCREEN_WIDTH)
        {
            gSonicScore++;
            if (gSonicScore >= WIN_SCORE) { gGameState = GAME_OVER; gWinner = 1; }
            else resetBall(i);
        }
    }
}

// ─── Render ───────────────────────────────────────────────────────────────────
void render()
{
    BeginDrawing();

    // Background stretched to fill screen
    {
        Rectangle src = { 0.0f, 0.0f,
                          (float)gTexBG.width, (float)gTexBG.height };
        Rectangle dst = { 0.0f, 0.0f,
                          (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
        Vector2   ori = { 0.0f, 0.0f };
        DrawTexturePro(gTexBG, src, dst, ori, 0.0f, WHITE);
    }

    // Scores
    DrawText(TextFormat("%d", gSonicScore), SCREEN_WIDTH / 4,     20, 60, YELLOW);
    DrawText(TextFormat("%d", gMarioScore), 3 * SCREEN_WIDTH / 4, 20, 60, RED);

    // Mode indicator (centred at top)
    if (gMarioIsAI)
        DrawText("1P MODE",
                 SCREEN_WIDTH / 2 - MeasureText("1P MODE", 30) / 2,
                 20, 30, WHITE);

    // Ball count hint at bottom
    const char *hint = TextFormat("Balls: %d  (press 1 / 2 / 3)", gActiveBalls);
    DrawText(hint,
             SCREEN_WIDTH / 2 - MeasureText(hint, 20) / 2,
             SCREEN_HEIGHT - 30, 20, WHITE);

    // Sonic paddle
    drawTex(gTexSonic, gSonicPos, gSonicScale);

    // Mario paddle
    drawTex(gTexMario, gMarioPos, gMarioScale);

    // Balls
    for (int i = 0; i < MAX_BALLS; i++)
    {
        if (!gBalls[i].active) continue;
        drawTex(gTexBall, gBalls[i].pos, gBalls[i].scale);
    }

    // Game Over overlay
    if (gGameState == GAME_OVER)
    {
        // Dark overlay
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 0, 0, 0, 160 });

        // end_message.png centred
        float   msgW     = (float)gTexEndMsg.width  * 2.0f;
        float   msgH     = (float)gTexEndMsg.height * 2.0f;
        Vector2 msgPos   = { ORIGIN.x, ORIGIN.y - 60.0f };
        Vector2 msgScale = { msgW, msgH };
        drawTex(gTexEndMsg, msgPos, msgScale);

        // Winner text
        const char *winText = (gWinner == 1) ? "SONIC WINS!" : "MARIO WINS!";
        Color       winCol  = (gWinner == 1) ? SKYBLUE : RED;
        DrawText(winText,
                 SCREEN_WIDTH / 2 - MeasureText(winText, 60) / 2,
                 (int)ORIGIN.y + 60,
                 60, winCol);

        DrawText("Press Q to quit",
                 SCREEN_WIDTH / 2 - MeasureText("Press Q to quit", 30) / 2,
                 (int)ORIGIN.y + 140,
                 30, WHITE);
    }

    EndDrawing();
}

// ─── Shutdown ─────────────────────────────────────────────────────────────────
void shutdown()
{
    UnloadTexture(gTexBG);
    UnloadTexture(gTexSonic);
    UnloadTexture(gTexMario);
    UnloadTexture(gTexBall);
    UnloadTexture(gTexEndMsg);
    CloseWindow();
}

// ─── Main ─────────────────────────────────────────────────────────────────────
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