#include "entity.h"

#ifndef SCENE_H
#define SCENE_H

struct GameState
{
    Entity *mario;
    Entity *enemies;
    int     enemyCount;
    Entity *coins;
    int     coinCount;
    Map    *map;

    Music  bgm;
    Sound  jumpSound;
    Sound  coinSound;
    Sound  hurtSound;

    int    nextSceneID;
    int    lives;
    int    coinsLeft;
};

class Scene
{
protected:
    GameState   mGameState;
    Vector2     mOrigin;
    const char *mBGColourHexCode = "#5C94FC";

public:
    Scene();
    Scene(Vector2 origin, const char *bgHexCode);

    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;

    GameState   getState()           const { return mGameState;       }
    GameState  *getStatePtr()              { return &mGameState;      }
    Vector2     getOrigin()          const { return mOrigin;          }
    const char *getBGColourHexCode() const { return mBGColourHexCode; }
};

#endif // SCENE_H