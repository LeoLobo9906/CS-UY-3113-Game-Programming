#ifndef DANCE_SCENE_H
#define DANCE_SCENE_H

#include "dance_cs3113.h"

struct Dancer
{
    Texture2D  texture;
    int        frameCount   = 4;
    int        currentFrame = 0;
    float      frameTimer   = 0.0f;
    float      frameSpeed   = 8.0f;
    Vector2    position;
    float      drawSize     = 192.0f;  // max rendered size
};

struct DanceGameState
{
    Music bgm;
    int   nextSceneID = 0;
    int   score       = 0;
};

class DanceScene
{
protected:
    DanceGameState mGameState;
    Vector2        mOrigin;
    const char    *mBGColourHexCode = "#7DD8F5";

    Texture2D mBgTile;
    Texture2D mBoxTex;
    Dancer    mDancer;

    void drawTiledBackground(int screenW, int screenH) const;
    void drawDancerBox() const;
    void updateDancer(float deltaTime);

public:
    DanceScene();
    DanceScene(Vector2 origin, const char *bgHexCode);

    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;

    DanceGameState  getState()    const { return mGameState;  }
    DanceGameState *getStatePtr()       { return &mGameState; }
    Vector2         getOrigin()   const { return mOrigin;     }
};

#endif // DANCE_SCENE_H