#include "scene.h"

#ifndef LEVELC_H
#define LEVELC_H

constexpr int LEVELC_WIDTH  = 30;
constexpr int LEVELC_HEIGHT = 8;

class LevelC : public Scene
{
private:
    unsigned int mLevelData[LEVELC_WIDTH * LEVELC_HEIGHT] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,
        0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,
        1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,
        0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,
        0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0
    };

public:
    static constexpr float TILE_SIZE   = 48.0f;
    static constexpr float GRAVITY     = 981.0f;
    static constexpr int   NUM_ENEMIES = 5;
    static constexpr int   NUM_COINS   = 8;

    LevelC();
    LevelC(Vector2 origin, const char *bgHexCode);
    ~LevelC();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;

    void setLives(int lives) { mGameState.lives = lives; }
};

#endif // LEVELC_H