#include "dance_scene.h"
#include <vector>

#ifndef DANCE_GAMESCREEN_H
#define DANCE_GAMESCREEN_H

enum ArrowDir { DIR_LEFT = 0, DIR_DOWN = 1, DIR_UP = 2, DIR_RIGHT = 3 };

enum Difficulty
{
    DIFFICULTY_EASY,
    DIFFICULTY_HARD
};

struct BeatNote
{
    float    time;
    ArrowDir dir;
};

enum GamePhase { PHASE_PLAYING, PHASE_RESULTS };

struct Note
{
    ArrowDir dir;
    float    y;
    bool     active;
    float    hitFlash;
};

enum HitRating { RATE_NONE, RATE_PERFECT, RATE_GOOD, RATE_MISS };

struct HitEffect
{
    HitRating rating  = RATE_NONE;
    float     timer   = 0.0f;
    float     rise    = 0.0f;
};

class DanceGameScreen : public DanceScene
{
private:
    float mTime = 0.0f;

    Texture2D mLaneBg;
    Texture2D mArrowTex;
    Texture2D mHighlightTex;

    Dancer mDancer2;

    float mPanelX, mPanelY, mPanelW, mPanelH, mScale;

    float mLaneCX[4];
    float mTargetY;
    float mSpawnY;
    float mArrowSz;

    std::vector<BeatNote> mBeatmap;
    int                   mBeatIndex  = 0;
    float                 mTravelTime = 0.0f;

    int mMaxCombo = 0;
    int mPerfects = 0;
    int mGoods    = 0;
    int mMisses   = 0;

    GamePhase mPhase = PHASE_PLAYING;

    Difficulty mDifficulty = DIFFICULTY_HARD;

    std::vector<Note> mNotes;
    float mNoteSpeed = 380.0f;

    HitEffect mHitFX[4];
    bool      mWasDown[4] = {};

    int mScore = 0;
    int mCombo = 0;

    static const int kArrowX[4];
    static const int kArrowW[4];
    static const int kArrowY[4];
    static const int kArrowH[4];

    void buildBeatmap();
    void buildEasyBeatmap();
    void buildHardBeatmap();

    void spawnNote(ArrowDir dir);
    void tryHit(ArrowDir dir);

    void drawDancerBox2() const;
    void drawPanel()      const;
    void drawTargets()    const;
    void drawNotes()      const;
    void drawFX()         const;
    void drawHUD()        const;
    void drawResults()    const;

public:
    DanceGameScreen();
    DanceGameScreen(Vector2 origin, const char *bgHexCode);
    ~DanceGameScreen();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;

    void setScore(int s) { mGameState.score = s; }
    void setDifficulty(Difficulty d) { mDifficulty = d; }
};

#endif
