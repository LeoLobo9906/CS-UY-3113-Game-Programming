#include "CS3113/cs3113.h"

// Global Constants
constexpr int SCREEN_WIDTH  = 1600,
              SCREEN_HEIGHT = 900,
              FPS           = 60,
              SIZE          = 500 / 2,
              SPEED         = 200;

constexpr char    BG_COLOUR[]    = "#F8F1C8";
constexpr Vector2 ORIGIN         = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
                  BASE_SIZE      = { (float) SIZE, (float) SIZE },
                  INIT_POS  = { ORIGIN.x - 200.0f, ORIGIN.y },
                  RUPEE_INIT_POS = { ORIGIN.x + 250.0f, ORIGIN.y };

// Images owned by Nintendo — please don't sue me.
constexpr char LINK_FP[]  = "assets/game/awakening-link.png";
constexpr char RUPEE_FP[] = "assets/game/awakening-rupee.png";

// Global Variables
AppStatus gAppStatus     = RUNNING;
float     gAngle         = 0.0f,
          gPreviousTicks = 0.0f;

Vector2 gPosition  = INIT_POS,
        gMovement  = { 0.0f, 0.0f },
        gScale     = BASE_SIZE,

        gRupeePosition = RUPEE_INIT_POS,
        gRupeeMovement = { 0.0f, 0.0f },
        gRupeeScale    = { BASE_SIZE.x / 2.0f , BASE_SIZE.y / 2.0f * 1.6f},

        gMousePosition = GetMousePosition();

Texture2D gLinkTexture;
Texture2D gRupeeTexture;

unsigned int startTime;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();
// Function Definitions

bool isColliding(const Vector2 *positionA,  const Vector2 *scaleA,
                 const Vector2 *positionB, const Vector2 *scaleB)
{
    float xDistance = fabs(positionA->x - positionB->x) - ((scaleA->x + scaleB->x) / 2.0f);
    float yDistance = fabs(positionA->y - positionB->y) - ((scaleA->y + scaleB->y) / 2.0f);

    if (xDistance < 0.0f && yDistance < 0.0f) return true;

    return false;
}

void renderObject(const Texture2D *texture, const Vector2 *position, 
                  const Vector2 *scale)
{
    // Whole texture (UV coordinates)
    Rectangle textureArea = {
        // top-left corner
        0.0f, 0.0f,

        // bottom-right corner (of texture)
        static_cast<float>(texture->width),
        static_cast<float>(texture->height)
    };

    // Destination rectangle – centred on gPosition
    Rectangle destinationArea = {
        position->x,
        position->y,
        static_cast<float>(scale->x),
        static_cast<float>(scale->y)
    };

    // Origin inside the source texture (centre of the texture)
    Vector2 originOffset = {
        static_cast<float>(scale->x) / 2.0f,
        static_cast<float>(scale->y) / 2.0f
    };

    // Render the texture on screen
    DrawTexturePro(
        *texture, 
        textureArea, destinationArea, originOffset,
        gAngle, WHITE
    );
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "User Input / Collision Detection");

    startTime = time(NULL);

    gLinkTexture  = LoadTexture(LINK_FP);
    gRupeeTexture = LoadTexture(RUPEE_FP);

    SetTargetFPS(FPS);
}

void processInput() {
    gMovement = {0.0f, 0.0f};

    if (IsKeyDown(KEY_W)) gMovement.y = -1; // Up
    else if (IsKeyDown(KEY_S)) gMovement.y = 1; //Down
    if (IsKeyDown(KEY_A)) gPosition.x = -1; // Left
    else if (IsKeyDown(KEY_D)) gMovement.x = 1; //Down

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (gPosition.x < gMousePosition.x) gMovement.x = 1;
        if (gPosition.x > gMousePosition.x) gMovement.x = -1;
        if (gPosition.y < gMousePosition.y) gMovement.y = 1;
        if (gPosition.y > gMousePosition.y) gMovement.y = -1;
    }
    if (GetLength(&gMovement) > 1.0f) {
        Normalise(&gMovement);
    }

    if (WindowShouldClose() || IsKeyPressed(KEY_Q)) gAppStatus = TERMINATED;


}

void update() 
{
    // Delta time
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    gPosition.x += SPEED * gMovement.x * deltaTime;
    gPosition.y += SPEED * gMovement.y * deltaTime;

    if (isColliding(&gPosition,&gScale, &gRupeePosition,  &gRupeeScale)) printf("Collision\n");

    gMousePosition = GetMousePosition();
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    // Render Link
    renderObject(&gLinkTexture, &gPosition, &gScale);

    // Render the rupee
    renderObject(&gRupeeTexture, &gRupeePosition, &gRupeeScale);

    EndDrawing();
}

void shutdown() 
{ 
    CloseWindow(); 
    UnloadTexture(gLinkTexture);
    UnloadTexture(gRupeeTexture);
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