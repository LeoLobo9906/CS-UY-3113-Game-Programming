#include "dance_scene.h"

// Per-frame source rects measured directly from the 400x100 sheet.
// Each character lives within its 100px column but may not fill it fully.
// src_x = absolute x in the sheet, w/h = tight char bounds, src_y = top of char.
struct FrameSrc { int src_x, src_y, w, h; };
static const FrameSrc FRAMES[4] = {
    {  32, 0, 68, 96 },   // frame 0: char at sheet x=32-99
    { 100, 8,100, 87 },   // frame 1: char at sheet x=100-199
    { 200, 8,100, 86 },   // frame 2: char at sheet x=200-299
    { 300, 0, 59, 94 },   // frame 3: char at sheet x=300-358
};

DanceScene::DanceScene()
    : mOrigin{0.0f, 0.0f}, mBGColourHexCode{nullptr} {}

DanceScene::DanceScene(Vector2 origin, const char *bgHexCode)
    : mOrigin{origin}, mBGColourHexCode{bgHexCode} {}

void DanceScene::drawTiledBackground(int screenW, int screenH) const
{
    if (mBgTile.id == 0) return;
    float tileSize = 120.0f;
    int cols = (int)(screenW / tileSize) + 2;
    int rows = (int)(screenH / tileSize) + 2;
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
        {
            Rectangle src = { 0, 0, (float)mBgTile.width, (float)mBgTile.height };
            Rectangle dst = { c * tileSize, r * tileSize, tileSize, tileSize };
            DrawTexturePro(mBgTile, src, dst, {0,0}, 0.0f, WHITE);
        }
}

void DanceScene::updateDancer(float deltaTime)
{
    mDancer.frameTimer += deltaTime;
    float spf = 1.0f / mDancer.frameSpeed;
    if (mDancer.frameTimer >= spf)
    {
        mDancer.frameTimer   = 0.0f;
        mDancer.currentFrame = (mDancer.currentFrame + 1) % mDancer.frameCount;
    }
}

void DanceScene::drawDancerBox() const
{
    // Draw the stage box (300x300) centred on dancer position
    if (mBoxTex.id != 0)
    {
        float bw = (float)mBoxTex.width  * 3.0f;
        float bh = (float)mBoxTex.height * 3.0f;
        Rectangle bSrc = { 0, 0, (float)mBoxTex.width, (float)mBoxTex.height };
        Rectangle bDst = { mDancer.position.x - bw/2.0f,
                           mDancer.position.y - bh/2.0f, bw, bh };
        DrawTexturePro(mBoxTex, bSrc, bDst, {0,0}, 0.0f, WHITE);
    }

    if (mDancer.texture.id == 0) return;

    const FrameSrc &f = FRAMES[mDancer.currentFrame];

    // Source: tight rect around just this frame's character pixels
    Rectangle src = { (float)f.src_x, (float)f.src_y,
                      (float)f.w,     (float)f.h };

    // Scale uniformly so the tallest dimension = drawSize
    float scale = mDancer.drawSize / (float)(f.h > f.w ? f.h : f.w);
    float dstW  = f.w * scale;
    float dstH  = f.h * scale;

    // Centre on dancer position
    Rectangle dst = { mDancer.position.x - dstW/2.0f,
                      mDancer.position.y - dstH/2.0f,
                      dstW, dstH };

    // BLACK is the sprite background — draw with colour key via tint trick:
    // We use BeginBlendMode(BLEND_CUSTOM) is complex, so instead we rely on
    // ImageColorReplace at load time (done in initialise of each scene).
    DrawTexturePro(mDancer.texture, src, dst, {0,0}, 0.0f, WHITE);
}