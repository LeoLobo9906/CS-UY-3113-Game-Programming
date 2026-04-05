#include "scene.h"

#ifndef GAMEOVER_H
#define GAMEOVER_H

class GameOver : public Scene
{
public:
    GameOver();
    GameOver(Vector2 origin, const char *bgHexCode);
    ~GameOver();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // GAMEOVER_H