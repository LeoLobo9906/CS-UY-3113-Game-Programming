#include "scene.h"

#ifndef LEVELA_H
#define LEVELA_H

constexpr int LEVEL_WIDTH  = 30;
constexpr int LEVEL_HEIGHT = 8;

class LevelA : public Scene
{
private:
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1
    };

public:
    static constexpr float TILE_SIZE           = 48.0f;
    static constexpr float GRAVITY             = 981.0f;
    static constexpr int   NUM_ENEMIES         = 3;
    static constexpr int   NUM_COINS           = 5;

    LevelA();
    LevelA(Vector2 origin, const char *bgHexCode);
    ~LevelA();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;

    void setLives(int lives) { mGameState.lives = lives; }
};

#endif // LEVELA_H