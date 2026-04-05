#include "scene.h"

#ifndef VICTORY_H
#define VICTORY_H

class Victory : public Scene
{
public:
    Victory();
    Victory(Vector2 origin, const char *bgHexCode);
    ~Victory();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // VICTORY_H