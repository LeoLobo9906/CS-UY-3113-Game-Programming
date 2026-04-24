#include "dance_scene.h"

#ifndef DANCE_MENU_H
#define DANCE_MENU_H

class DanceMenu : public DanceScene
{
private:
    Shader mDancerShader;
    int    mShaderTimeLoc;
    float mTitlePulse = 0.0f;
    int   mSelectedOption = 0; // 0 = Easy, 1 = Hard

public:
    DanceMenu();
    DanceMenu(Vector2 origin, const char *bgHexCode);
    ~DanceMenu();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // DANCE_MENU_H
