#include "scene.h"

#ifndef LEVELB_H
#define LEVELB_H

constexpr int LEVELB_WIDTH  = 30;
constexpr int LEVELB_HEIGHT = 8;

class LevelB : public Scene
{
private:
    unsigned int mLevelData[LEVELB_WIDTH * LEVELB_HEIGHT] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,
        0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
        1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1
    };

public:
    static constexpr float TILE_SIZE   = 48.0f;
    static constexpr float GRAVITY     = 981.0f;
    static constexpr int   NUM_ENEMIES = 4;
    static constexpr int   NUM_COINS   = 6;

    LevelB();
    LevelB(Vector2 origin, const char *bgHexCode);
    ~LevelB();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;

    void setLives(int lives) { mGameState.lives = lives; }
};

#endif // LEVELB_H