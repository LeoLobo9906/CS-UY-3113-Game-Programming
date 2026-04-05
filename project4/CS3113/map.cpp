#include "map.h"

Map::Map(int mapColumns, int mapRows, unsigned int *levelData,
         const char *textureFilePath, float tileSize,
         int textureColumns, int textureRows, Vector2 origin)
    : mMapColumns{mapColumns}, mMapRows{mapRows},
      mTextureAtlas{LoadTexture(textureFilePath)},
      mLevelData{levelData}, mTileSize{tileSize},
      mTextureColumns{textureColumns}, mTextureRows{textureRows},
      mOrigin{origin}
{ build(); }

Map::~Map() { UnloadTexture(mTextureAtlas); }

void Map::build()
{
    mLeftBoundary   = mOrigin.x - (mMapColumns * mTileSize) / 2.0f;
    mRightBoundary  = mOrigin.x + (mMapColumns * mTileSize) / 2.0f;
    mTopBoundary    = mOrigin.y - (mMapRows    * mTileSize) / 2.0f;
    mBottomBoundary = mOrigin.y + (mMapRows    * mTileSize) / 2.0f;

    for (int row = 0; row < mTextureRows; row++)
    {
        for (int col = 0; col < mTextureColumns; col++)
        {
            Rectangle textureArea = {
                (float)col * (mTextureAtlas.width  / mTextureColumns),
                (float)row * (mTextureAtlas.height / mTextureRows),
                (float)mTextureAtlas.width  / mTextureColumns,
                (float)mTextureAtlas.height / mTextureRows
            };
            mTextureAreas.push_back(textureArea);
        }
    }
}

void Map::render()
{
    for (int row = 0; row < mMapRows; row++)
    {
        for (int col = 0; col < mMapColumns; col++)
        {
            int tile = mLevelData[row * mMapColumns + col];
            if (tile == 0) continue;

            Rectangle dst = {
                mLeftBoundary + col * mTileSize,
                mTopBoundary  + row * mTileSize,
                mTileSize, mTileSize
            };
            DrawTexturePro(mTextureAtlas,
                           mTextureAreas[tile - 1],
                           dst, {0.0f, 0.0f}, 0.0f, WHITE);
        }
    }
}

bool Map::isSolidTileAt(Vector2 position, float *xOverlap, float *yOverlap)
{
    *xOverlap = 0.0f;
    *yOverlap = 0.0f;

    if (position.x < mLeftBoundary  || position.x > mRightBoundary ||
        position.y < mTopBoundary   || position.y > mBottomBoundary)
        return false;

    int tileX = (int)floor((position.x - mLeftBoundary) / mTileSize);
    int tileY = (int)floor((position.y - mTopBoundary)  / mTileSize);

    if (tileX < 0 || tileX >= mMapColumns ||
        tileY < 0 || tileY >= mMapRows)
        return false;

    int tile = mLevelData[tileY * mMapColumns + tileX];
    if (tile == 0) return false;

    float cx = mLeftBoundary + tileX * mTileSize + mTileSize / 2.0f;
    float cy = mTopBoundary  + tileY * mTileSize + mTileSize / 2.0f;

    *xOverlap = fmaxf(0.0f, (mTileSize / 2.0f) - fabs(position.x - cx));
    *yOverlap = fmaxf(0.0f, (mTileSize / 2.0f) - fabs(position.y - cy));

    return true;
}