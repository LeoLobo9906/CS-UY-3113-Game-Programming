#include "dance_gamescreen.h"
#include <algorithm>
#include <cmath>
#include <cstdio>

// Measured from arrows.png (990×252) — 4 arrow outlines
const int DanceGameScreen::kArrowX[4] = {  17, 286, 531, 768 };
const int DanceGameScreen::kArrowW[4] = { 209, 176, 177, 208 };
const int DanceGameScreen::kArrowY[4] = {  32,   8,   8,  32 };
const int DanceGameScreen::kArrowH[4] = { 184, 216, 216, 184 };

DanceGameScreen::DanceGameScreen() : DanceScene{{0.0f}, nullptr} {}
DanceGameScreen::DanceGameScreen(Vector2 o, const char *bg) : DanceScene{o, bg} {}
DanceGameScreen::~DanceGameScreen() { shutdown(); }

// BEATMAP
// { seconds_into_song,  direction }
// Directions: DIR_LEFT  DIR_DOWN  DIR_UP  DIR_RIGHT
void DanceGameScreen::buildBeatmap()
{
    if (mDifficulty == DIFFICULTY_EASY)
        buildEasyBeatmap();
    else
        buildHardBeatmap();
}

void DanceGameScreen::buildEasyBeatmap()
{
    mBeatmap.clear();

    // Easy chart: still follows the detected song beats,
    // but uses fewer notes and no double arrows.

    // ── EASY VERSE 1 / INTRO GROOVE (~0:01-0:31) ───────────────
    mBeatmap.push_back({  3.88f, DIR_UP   });
    mBeatmap.push_back({  6.73f, DIR_RIGHT});
    mBeatmap.push_back({  9.61f, DIR_DOWN });
    mBeatmap.push_back({ 12.44f, DIR_LEFT });
    mBeatmap.push_back({ 15.32f, DIR_UP   });
    mBeatmap.push_back({ 17.99f, DIR_DOWN });
    mBeatmap.push_back({ 19.59f, DIR_RIGHT});
    mBeatmap.push_back({ 21.04f, DIR_LEFT });
    mBeatmap.push_back({ 22.47f, DIR_UP   });
    mBeatmap.push_back({ 23.87f, DIR_RIGHT});
    mBeatmap.push_back({ 25.32f, DIR_DOWN });
    mBeatmap.push_back({ 26.75f, DIR_LEFT });
    mBeatmap.push_back({ 28.18f, DIR_UP   });
    mBeatmap.push_back({ 29.62f, DIR_DOWN });
    // ── EASY CHORUS 1 (~0:31-0:48) ───────────────────────────
    mBeatmap.push_back({ 31.03f, DIR_RIGHT});
    mBeatmap.push_back({ 31.73f, DIR_LEFT });
    mBeatmap.push_back({ 32.45f, DIR_UP   });
    mBeatmap.push_back({ 33.17f, DIR_RIGHT});
    mBeatmap.push_back({ 33.88f, DIR_DOWN });
    mBeatmap.push_back({ 34.59f, DIR_LEFT });
    mBeatmap.push_back({ 35.31f, DIR_UP   });
    mBeatmap.push_back({ 36.03f, DIR_DOWN });
    mBeatmap.push_back({ 36.75f, DIR_RIGHT});
    mBeatmap.push_back({ 37.47f, DIR_LEFT });
    mBeatmap.push_back({ 38.18f, DIR_UP   });
    mBeatmap.push_back({ 38.88f, DIR_RIGHT});
    mBeatmap.push_back({ 39.60f, DIR_DOWN });
    mBeatmap.push_back({ 40.32f, DIR_LEFT });
    mBeatmap.push_back({ 41.03f, DIR_UP   });
    mBeatmap.push_back({ 41.74f, DIR_DOWN });
    mBeatmap.push_back({ 42.46f, DIR_RIGHT});
    mBeatmap.push_back({ 43.18f, DIR_LEFT });
    mBeatmap.push_back({ 43.89f, DIR_UP   });
    mBeatmap.push_back({ 44.60f, DIR_RIGHT});
    mBeatmap.push_back({ 45.31f, DIR_DOWN });
    mBeatmap.push_back({ 46.04f, DIR_LEFT });
    mBeatmap.push_back({ 46.76f, DIR_UP   });
    mBeatmap.push_back({ 47.47f, DIR_DOWN });
    // ── EASY VERSE 2 (~0:48-1:12) ─────────────────────────────
    mBeatmap.push_back({ 48.20f, DIR_RIGHT});
    mBeatmap.push_back({ 49.25f, DIR_LEFT });
    mBeatmap.push_back({ 50.32f, DIR_UP   });
    mBeatmap.push_back({ 51.39f, DIR_RIGHT});
    mBeatmap.push_back({ 52.45f, DIR_DOWN });
    mBeatmap.push_back({ 53.54f, DIR_LEFT });
    mBeatmap.push_back({ 54.60f, DIR_UP   });
    mBeatmap.push_back({ 55.68f, DIR_DOWN });
    mBeatmap.push_back({ 56.75f, DIR_RIGHT});
    mBeatmap.push_back({ 57.82f, DIR_LEFT });
    mBeatmap.push_back({ 58.89f, DIR_UP   });
    mBeatmap.push_back({ 59.96f, DIR_RIGHT});
    mBeatmap.push_back({ 61.03f, DIR_DOWN });
    mBeatmap.push_back({ 62.11f, DIR_LEFT });
    mBeatmap.push_back({ 63.18f, DIR_UP   });
    mBeatmap.push_back({ 64.24f, DIR_DOWN });
    mBeatmap.push_back({ 65.33f, DIR_RIGHT});
    mBeatmap.push_back({ 66.39f, DIR_LEFT });
    mBeatmap.push_back({ 67.45f, DIR_UP   });
    mBeatmap.push_back({ 68.53f, DIR_RIGHT});
    mBeatmap.push_back({ 69.59f, DIR_DOWN });
    mBeatmap.push_back({ 70.67f, DIR_LEFT });
    mBeatmap.push_back({ 71.75f, DIR_UP   });
    mBeatmap.push_back({ 72.83f, DIR_DOWN });
    mBeatmap.push_back({ 73.89f, DIR_RIGHT});
    mBeatmap.push_back({ 74.95f, DIR_LEFT });
    mBeatmap.push_back({ 76.04f, DIR_UP   });
    mBeatmap.push_back({ 77.10f, DIR_RIGHT});
    mBeatmap.push_back({ 78.19f, DIR_DOWN });
    mBeatmap.push_back({ 79.24f, DIR_LEFT });
    mBeatmap.push_back({ 80.32f, DIR_UP   });
    mBeatmap.push_back({ 81.39f, DIR_DOWN });
    mBeatmap.push_back({ 82.47f, DIR_RIGHT});
    mBeatmap.push_back({ 83.54f, DIR_LEFT });
    mBeatmap.push_back({ 84.61f, DIR_UP   });
    mBeatmap.push_back({ 85.67f, DIR_RIGHT});
    // ── EASY FINAL CHORUS (~1:27-1:49) ────────────────────────
    mBeatmap.push_back({ 86.73f, DIR_DOWN });
    mBeatmap.push_back({ 87.45f, DIR_LEFT });
    mBeatmap.push_back({ 88.16f, DIR_UP   });
    mBeatmap.push_back({ 88.88f, DIR_DOWN });
    mBeatmap.push_back({ 89.60f, DIR_RIGHT});
    mBeatmap.push_back({ 90.32f, DIR_LEFT });
    mBeatmap.push_back({ 91.03f, DIR_UP   });
    mBeatmap.push_back({ 91.75f, DIR_RIGHT});
    mBeatmap.push_back({ 92.46f, DIR_DOWN });
    mBeatmap.push_back({ 93.18f, DIR_LEFT });
    mBeatmap.push_back({ 93.89f, DIR_UP   });
    mBeatmap.push_back({ 94.60f, DIR_DOWN });
    mBeatmap.push_back({ 95.32f, DIR_RIGHT});
    mBeatmap.push_back({ 96.02f, DIR_LEFT });
    mBeatmap.push_back({ 96.74f, DIR_UP   });
    mBeatmap.push_back({ 97.45f, DIR_RIGHT});
    mBeatmap.push_back({ 98.17f, DIR_DOWN });
    mBeatmap.push_back({ 98.88f, DIR_LEFT });
    mBeatmap.push_back({ 99.61f, DIR_UP   });
    mBeatmap.push_back({100.32f, DIR_DOWN });
    mBeatmap.push_back({101.04f, DIR_RIGHT});
    mBeatmap.push_back({101.74f, DIR_LEFT });
    mBeatmap.push_back({102.46f, DIR_UP   });
    mBeatmap.push_back({103.17f, DIR_RIGHT});
    mBeatmap.push_back({103.89f, DIR_DOWN });
    mBeatmap.push_back({104.61f, DIR_LEFT });
    mBeatmap.push_back({105.31f, DIR_UP   });
    mBeatmap.push_back({106.03f, DIR_DOWN });
    mBeatmap.push_back({106.74f, DIR_RIGHT});
    mBeatmap.push_back({107.46f, DIR_LEFT });
    mBeatmap.push_back({108.17f, DIR_UP   });

    std::sort(mBeatmap.begin(), mBeatmap.end(),
              [](const BeatNote &a, const BeatNote &b)
              {
                  return a.time < b.time;
              });
}

void DanceGameScreen::buildHardBeatmap()
{
    mBeatmap.clear();

    // ── VERSE 1 / INTRO GROOVE (~0:01-0:31) ─────────────────────
    mBeatmap.push_back({  2.43f, DIR_DOWN });
    mBeatmap.push_back({  3.88f, DIR_UP   });
    mBeatmap.push_back({  5.32f, DIR_RIGHT});
    mBeatmap.push_back({  6.73f, DIR_UP   });
    mBeatmap.push_back({  6.73f, DIR_LEFT });
    mBeatmap.push_back({  8.17f, DIR_DOWN });
    mBeatmap.push_back({  8.17f, DIR_UP   });
    mBeatmap.push_back({  9.61f, DIR_UP   });
    mBeatmap.push_back({ 11.03f, DIR_RIGHT});
    mBeatmap.push_back({ 12.44f, DIR_UP   });
    mBeatmap.push_back({ 12.44f, DIR_LEFT });
    mBeatmap.push_back({ 13.89f, DIR_DOWN });
    mBeatmap.push_back({ 15.32f, DIR_UP   });
    mBeatmap.push_back({ 16.74f, DIR_RIGHT});
    mBeatmap.push_back({ 17.99f, DIR_UP   });
    mBeatmap.push_back({ 17.99f, DIR_LEFT });
    mBeatmap.push_back({ 18.44f, DIR_RIGHT});
    mBeatmap.push_back({ 18.88f, DIR_DOWN });
    mBeatmap.push_back({ 18.88f, DIR_UP   });
    mBeatmap.push_back({ 19.25f, DIR_LEFT });
    mBeatmap.push_back({ 19.59f, DIR_UP   });
    mBeatmap.push_back({ 19.97f, DIR_DOWN });
    mBeatmap.push_back({ 20.32f, DIR_RIGHT});
    mBeatmap.push_back({ 20.68f, DIR_LEFT });
    mBeatmap.push_back({ 21.04f, DIR_UP   });
    mBeatmap.push_back({ 21.04f, DIR_LEFT });
    mBeatmap.push_back({ 21.39f, DIR_RIGHT});
    mBeatmap.push_back({ 21.75f, DIR_DOWN });
    mBeatmap.push_back({ 22.11f, DIR_LEFT });
    mBeatmap.push_back({ 22.47f, DIR_UP   });
    mBeatmap.push_back({ 22.82f, DIR_DOWN });
    mBeatmap.push_back({ 23.18f, DIR_RIGHT});
    mBeatmap.push_back({ 23.52f, DIR_LEFT });
    mBeatmap.push_back({ 23.87f, DIR_UP   });
    mBeatmap.push_back({ 23.87f, DIR_LEFT });
    mBeatmap.push_back({ 24.24f, DIR_RIGHT});
    mBeatmap.push_back({ 24.59f, DIR_DOWN });
    mBeatmap.push_back({ 24.59f, DIR_UP   });
    mBeatmap.push_back({ 24.96f, DIR_LEFT });
    mBeatmap.push_back({ 25.32f, DIR_UP   });
    mBeatmap.push_back({ 25.67f, DIR_DOWN });
    mBeatmap.push_back({ 26.04f, DIR_RIGHT});
    mBeatmap.push_back({ 26.38f, DIR_LEFT });
    mBeatmap.push_back({ 26.75f, DIR_UP   });
    mBeatmap.push_back({ 26.75f, DIR_LEFT });
    mBeatmap.push_back({ 27.10f, DIR_RIGHT});
    mBeatmap.push_back({ 27.45f, DIR_DOWN });
    mBeatmap.push_back({ 27.81f, DIR_LEFT });
    mBeatmap.push_back({ 28.18f, DIR_UP   });
    mBeatmap.push_back({ 28.53f, DIR_DOWN });
    mBeatmap.push_back({ 28.89f, DIR_RIGHT});
    mBeatmap.push_back({ 29.26f, DIR_LEFT });
    mBeatmap.push_back({ 29.62f, DIR_UP   });
    mBeatmap.push_back({ 29.62f, DIR_LEFT });
    mBeatmap.push_back({ 29.96f, DIR_RIGHT});
    mBeatmap.push_back({ 30.32f, DIR_DOWN });
    mBeatmap.push_back({ 30.32f, DIR_UP   });
    mBeatmap.push_back({ 30.67f, DIR_LEFT });
    // ── CHORUS 1 (~0:31-0:48) ─────────────────────────────────
    mBeatmap.push_back({ 31.03f, DIR_UP   });
    mBeatmap.push_back({ 31.03f, DIR_LEFT });
    mBeatmap.push_back({ 31.39f, DIR_DOWN });
    mBeatmap.push_back({ 31.73f, DIR_LEFT });
    mBeatmap.push_back({ 31.73f, DIR_DOWN });
    mBeatmap.push_back({ 32.09f, DIR_RIGHT});
    mBeatmap.push_back({ 32.45f, DIR_LEFT });
    mBeatmap.push_back({ 32.45f, DIR_UP   });
    mBeatmap.push_back({ 32.81f, DIR_DOWN });
    mBeatmap.push_back({ 33.17f, DIR_UP   });
    mBeatmap.push_back({ 33.52f, DIR_RIGHT});
    mBeatmap.push_back({ 33.88f, DIR_UP   });
    mBeatmap.push_back({ 33.88f, DIR_LEFT });
    mBeatmap.push_back({ 34.24f, DIR_DOWN });
    mBeatmap.push_back({ 34.59f, DIR_LEFT });
    mBeatmap.push_back({ 34.59f, DIR_DOWN });
    mBeatmap.push_back({ 34.95f, DIR_RIGHT});
    mBeatmap.push_back({ 35.31f, DIR_LEFT });
    mBeatmap.push_back({ 35.31f, DIR_UP   });
    mBeatmap.push_back({ 35.67f, DIR_DOWN });
    mBeatmap.push_back({ 36.03f, DIR_UP   });
    mBeatmap.push_back({ 36.40f, DIR_RIGHT});
    mBeatmap.push_back({ 36.75f, DIR_UP   });
    mBeatmap.push_back({ 36.75f, DIR_LEFT });
    mBeatmap.push_back({ 37.11f, DIR_DOWN });
    mBeatmap.push_back({ 37.47f, DIR_LEFT });
    mBeatmap.push_back({ 37.47f, DIR_DOWN });
    mBeatmap.push_back({ 37.81f, DIR_RIGHT});
    mBeatmap.push_back({ 38.18f, DIR_LEFT });
    mBeatmap.push_back({ 38.18f, DIR_UP   });
    mBeatmap.push_back({ 38.53f, DIR_DOWN });
    mBeatmap.push_back({ 38.88f, DIR_UP   });
    mBeatmap.push_back({ 39.24f, DIR_RIGHT});
    mBeatmap.push_back({ 39.60f, DIR_UP   });
    mBeatmap.push_back({ 39.60f, DIR_LEFT });
    mBeatmap.push_back({ 39.95f, DIR_DOWN });
    mBeatmap.push_back({ 40.32f, DIR_LEFT });
    mBeatmap.push_back({ 40.32f, DIR_DOWN });
    mBeatmap.push_back({ 40.67f, DIR_RIGHT});
    mBeatmap.push_back({ 41.03f, DIR_LEFT });
    mBeatmap.push_back({ 41.03f, DIR_UP   });
    mBeatmap.push_back({ 41.39f, DIR_DOWN });
    mBeatmap.push_back({ 41.74f, DIR_UP   });
    mBeatmap.push_back({ 42.11f, DIR_RIGHT});
    mBeatmap.push_back({ 42.46f, DIR_UP   });
    mBeatmap.push_back({ 42.46f, DIR_LEFT });
    mBeatmap.push_back({ 42.82f, DIR_DOWN });
    mBeatmap.push_back({ 43.18f, DIR_LEFT });
    mBeatmap.push_back({ 43.18f, DIR_DOWN });
    mBeatmap.push_back({ 43.53f, DIR_RIGHT});
    mBeatmap.push_back({ 43.89f, DIR_LEFT });
    mBeatmap.push_back({ 43.89f, DIR_UP   });
    mBeatmap.push_back({ 44.25f, DIR_DOWN });
    mBeatmap.push_back({ 44.60f, DIR_UP   });
    mBeatmap.push_back({ 44.95f, DIR_RIGHT});
    mBeatmap.push_back({ 45.31f, DIR_UP   });
    mBeatmap.push_back({ 45.31f, DIR_LEFT });
    mBeatmap.push_back({ 45.68f, DIR_DOWN });
    mBeatmap.push_back({ 46.04f, DIR_LEFT });
    mBeatmap.push_back({ 46.04f, DIR_DOWN });
    mBeatmap.push_back({ 46.41f, DIR_RIGHT});
    mBeatmap.push_back({ 46.76f, DIR_LEFT });
    mBeatmap.push_back({ 46.76f, DIR_UP   });
    mBeatmap.push_back({ 47.11f, DIR_DOWN });
    mBeatmap.push_back({ 47.47f, DIR_UP   });
    mBeatmap.push_back({ 47.83f, DIR_RIGHT});
    // ── VERSE 2 (~0:48-1:12) ───────────────────────────────────
    mBeatmap.push_back({ 48.20f, DIR_UP   });
    mBeatmap.push_back({ 48.54f, DIR_DOWN });
    mBeatmap.push_back({ 48.89f, DIR_RIGHT});
    mBeatmap.push_back({ 49.25f, DIR_LEFT });
    mBeatmap.push_back({ 49.60f, DIR_UP   });
    mBeatmap.push_back({ 49.60f, DIR_LEFT });
    mBeatmap.push_back({ 49.96f, DIR_RIGHT});
    mBeatmap.push_back({ 50.32f, DIR_DOWN });
    mBeatmap.push_back({ 50.68f, DIR_LEFT });
    mBeatmap.push_back({ 51.04f, DIR_UP   });
    mBeatmap.push_back({ 51.39f, DIR_DOWN });
    mBeatmap.push_back({ 51.74f, DIR_RIGHT});
    mBeatmap.push_back({ 52.10f, DIR_LEFT });
    mBeatmap.push_back({ 52.45f, DIR_UP   });
    mBeatmap.push_back({ 52.45f, DIR_LEFT });
    mBeatmap.push_back({ 52.81f, DIR_RIGHT});
    mBeatmap.push_back({ 53.17f, DIR_DOWN });
    mBeatmap.push_back({ 53.17f, DIR_UP   });
    mBeatmap.push_back({ 53.54f, DIR_LEFT });
    mBeatmap.push_back({ 53.90f, DIR_UP   });
    mBeatmap.push_back({ 54.25f, DIR_DOWN });
    mBeatmap.push_back({ 54.60f, DIR_RIGHT});
    mBeatmap.push_back({ 54.97f, DIR_LEFT });
    mBeatmap.push_back({ 55.32f, DIR_UP   });
    mBeatmap.push_back({ 55.32f, DIR_LEFT });
    mBeatmap.push_back({ 55.68f, DIR_RIGHT});
    mBeatmap.push_back({ 56.04f, DIR_DOWN });
    mBeatmap.push_back({ 56.39f, DIR_LEFT });
    mBeatmap.push_back({ 56.75f, DIR_UP   });
    mBeatmap.push_back({ 57.10f, DIR_DOWN });
    mBeatmap.push_back({ 57.46f, DIR_RIGHT});
    mBeatmap.push_back({ 57.82f, DIR_LEFT });
    mBeatmap.push_back({ 58.18f, DIR_UP   });
    mBeatmap.push_back({ 58.18f, DIR_LEFT });
    mBeatmap.push_back({ 58.53f, DIR_RIGHT});
    mBeatmap.push_back({ 58.89f, DIR_DOWN });
    mBeatmap.push_back({ 58.89f, DIR_UP   });
    mBeatmap.push_back({ 59.25f, DIR_LEFT });
    mBeatmap.push_back({ 59.61f, DIR_UP   });
    mBeatmap.push_back({ 59.96f, DIR_DOWN });
    mBeatmap.push_back({ 60.31f, DIR_RIGHT});
    mBeatmap.push_back({ 60.68f, DIR_LEFT });
    mBeatmap.push_back({ 61.03f, DIR_UP   });
    mBeatmap.push_back({ 61.03f, DIR_LEFT });
    mBeatmap.push_back({ 61.39f, DIR_RIGHT});
    mBeatmap.push_back({ 61.74f, DIR_DOWN });
    mBeatmap.push_back({ 62.11f, DIR_LEFT });
    mBeatmap.push_back({ 62.47f, DIR_UP   });
    mBeatmap.push_back({ 62.82f, DIR_DOWN });
    mBeatmap.push_back({ 63.18f, DIR_RIGHT});
    mBeatmap.push_back({ 63.53f, DIR_LEFT });
    mBeatmap.push_back({ 63.89f, DIR_UP   });
    mBeatmap.push_back({ 63.89f, DIR_LEFT });
    mBeatmap.push_back({ 64.24f, DIR_RIGHT});
    mBeatmap.push_back({ 64.59f, DIR_DOWN });
    mBeatmap.push_back({ 64.59f, DIR_UP   });
    mBeatmap.push_back({ 64.96f, DIR_LEFT });
    mBeatmap.push_back({ 65.33f, DIR_UP   });
    mBeatmap.push_back({ 65.68f, DIR_DOWN });
    mBeatmap.push_back({ 66.03f, DIR_RIGHT});
    mBeatmap.push_back({ 66.39f, DIR_LEFT });
    mBeatmap.push_back({ 66.75f, DIR_UP   });
    mBeatmap.push_back({ 66.75f, DIR_LEFT });
    mBeatmap.push_back({ 67.10f, DIR_RIGHT});
    mBeatmap.push_back({ 67.45f, DIR_DOWN });
    mBeatmap.push_back({ 67.82f, DIR_LEFT });
    mBeatmap.push_back({ 68.18f, DIR_UP   });
    mBeatmap.push_back({ 68.53f, DIR_DOWN });
    mBeatmap.push_back({ 68.88f, DIR_RIGHT});
    mBeatmap.push_back({ 69.24f, DIR_LEFT });
    mBeatmap.push_back({ 69.59f, DIR_UP   });
    mBeatmap.push_back({ 69.59f, DIR_LEFT });
    mBeatmap.push_back({ 69.94f, DIR_RIGHT});
    mBeatmap.push_back({ 70.31f, DIR_DOWN });
    mBeatmap.push_back({ 70.31f, DIR_UP   });
    mBeatmap.push_back({ 70.67f, DIR_LEFT });
    mBeatmap.push_back({ 71.03f, DIR_UP   });
    mBeatmap.push_back({ 71.39f, DIR_DOWN });
    // ── VERSE BREAK (~1:12-1:27) ───────────────────────────────
    mBeatmap.push_back({ 71.75f, DIR_RIGHT});
    mBeatmap.push_back({ 72.11f, DIR_LEFT });
    mBeatmap.push_back({ 72.46f, DIR_UP   });
    mBeatmap.push_back({ 72.46f, DIR_LEFT });
    mBeatmap.push_back({ 72.83f, DIR_RIGHT});
    mBeatmap.push_back({ 73.18f, DIR_DOWN });
    mBeatmap.push_back({ 73.54f, DIR_LEFT });
    mBeatmap.push_back({ 73.89f, DIR_UP   });
    mBeatmap.push_back({ 74.25f, DIR_DOWN });
    mBeatmap.push_back({ 74.60f, DIR_RIGHT});
    mBeatmap.push_back({ 74.95f, DIR_LEFT });
    mBeatmap.push_back({ 75.32f, DIR_UP   });
    mBeatmap.push_back({ 75.32f, DIR_LEFT });
    mBeatmap.push_back({ 75.67f, DIR_RIGHT});
    mBeatmap.push_back({ 76.04f, DIR_DOWN });
    mBeatmap.push_back({ 76.04f, DIR_UP   });
    mBeatmap.push_back({ 76.39f, DIR_LEFT });
    mBeatmap.push_back({ 76.76f, DIR_UP   });
    mBeatmap.push_back({ 77.10f, DIR_DOWN });
    mBeatmap.push_back({ 77.46f, DIR_RIGHT});
    mBeatmap.push_back({ 77.82f, DIR_LEFT });
    mBeatmap.push_back({ 78.19f, DIR_UP   });
    mBeatmap.push_back({ 78.19f, DIR_LEFT });
    mBeatmap.push_back({ 78.53f, DIR_RIGHT});
    mBeatmap.push_back({ 78.89f, DIR_DOWN });
    mBeatmap.push_back({ 79.24f, DIR_LEFT });
    mBeatmap.push_back({ 79.60f, DIR_UP   });
    mBeatmap.push_back({ 79.96f, DIR_DOWN });
    mBeatmap.push_back({ 80.32f, DIR_RIGHT});
    mBeatmap.push_back({ 80.68f, DIR_LEFT });
    mBeatmap.push_back({ 81.04f, DIR_UP   });
    mBeatmap.push_back({ 81.04f, DIR_LEFT });
    mBeatmap.push_back({ 81.39f, DIR_RIGHT});
    mBeatmap.push_back({ 81.75f, DIR_DOWN });
    mBeatmap.push_back({ 81.75f, DIR_UP   });
    mBeatmap.push_back({ 82.11f, DIR_LEFT });
    mBeatmap.push_back({ 82.47f, DIR_UP   });
    mBeatmap.push_back({ 82.82f, DIR_DOWN });
    mBeatmap.push_back({ 83.18f, DIR_RIGHT});
    mBeatmap.push_back({ 83.54f, DIR_LEFT });
    mBeatmap.push_back({ 83.89f, DIR_UP   });
    mBeatmap.push_back({ 83.89f, DIR_LEFT });
    mBeatmap.push_back({ 84.25f, DIR_RIGHT});
    mBeatmap.push_back({ 84.61f, DIR_DOWN });
    mBeatmap.push_back({ 84.96f, DIR_LEFT });
    mBeatmap.push_back({ 85.32f, DIR_UP   });
    mBeatmap.push_back({ 85.67f, DIR_DOWN });
    mBeatmap.push_back({ 86.02f, DIR_RIGHT});
    mBeatmap.push_back({ 86.38f, DIR_LEFT });
    // ── FINAL CHORUS (~1:27-1:49) ──────────────────────────────
    mBeatmap.push_back({ 86.73f, DIR_LEFT });
    mBeatmap.push_back({ 86.73f, DIR_UP   });
    mBeatmap.push_back({ 87.10f, DIR_DOWN });
    mBeatmap.push_back({ 87.45f, DIR_UP   });
    mBeatmap.push_back({ 87.81f, DIR_RIGHT});
    mBeatmap.push_back({ 88.16f, DIR_UP   });
    mBeatmap.push_back({ 88.16f, DIR_LEFT });
    mBeatmap.push_back({ 88.52f, DIR_DOWN });
    mBeatmap.push_back({ 88.88f, DIR_LEFT });
    mBeatmap.push_back({ 88.88f, DIR_DOWN });
    mBeatmap.push_back({ 89.24f, DIR_RIGHT});
    mBeatmap.push_back({ 89.60f, DIR_LEFT });
    mBeatmap.push_back({ 89.60f, DIR_UP   });
    mBeatmap.push_back({ 89.95f, DIR_DOWN });
    mBeatmap.push_back({ 90.32f, DIR_UP   });
    mBeatmap.push_back({ 90.67f, DIR_RIGHT});
    mBeatmap.push_back({ 91.03f, DIR_UP   });
    mBeatmap.push_back({ 91.03f, DIR_LEFT });
    mBeatmap.push_back({ 91.39f, DIR_DOWN });
    mBeatmap.push_back({ 91.75f, DIR_LEFT });
    mBeatmap.push_back({ 91.75f, DIR_DOWN });
    mBeatmap.push_back({ 92.11f, DIR_RIGHT});
    mBeatmap.push_back({ 92.46f, DIR_LEFT });
    mBeatmap.push_back({ 92.46f, DIR_UP   });
    mBeatmap.push_back({ 92.82f, DIR_DOWN });
    mBeatmap.push_back({ 93.18f, DIR_UP   });
    mBeatmap.push_back({ 93.53f, DIR_RIGHT});
    mBeatmap.push_back({ 93.89f, DIR_UP   });
    mBeatmap.push_back({ 93.89f, DIR_LEFT });
    mBeatmap.push_back({ 94.24f, DIR_DOWN });
    mBeatmap.push_back({ 94.60f, DIR_LEFT });
    mBeatmap.push_back({ 94.60f, DIR_DOWN });
    mBeatmap.push_back({ 94.96f, DIR_RIGHT});
    mBeatmap.push_back({ 95.32f, DIR_LEFT });
    mBeatmap.push_back({ 95.32f, DIR_UP   });
    mBeatmap.push_back({ 95.67f, DIR_DOWN });
    mBeatmap.push_back({ 96.02f, DIR_UP   });
    mBeatmap.push_back({ 96.38f, DIR_RIGHT});
    mBeatmap.push_back({ 96.74f, DIR_UP   });
    mBeatmap.push_back({ 96.74f, DIR_LEFT });
    mBeatmap.push_back({ 97.10f, DIR_DOWN });
    mBeatmap.push_back({ 97.45f, DIR_LEFT });
    mBeatmap.push_back({ 97.45f, DIR_DOWN });
    mBeatmap.push_back({ 97.81f, DIR_RIGHT});
    mBeatmap.push_back({ 98.17f, DIR_LEFT });
    mBeatmap.push_back({ 98.17f, DIR_UP   });
    mBeatmap.push_back({ 98.53f, DIR_DOWN });
    mBeatmap.push_back({ 98.88f, DIR_UP   });
    mBeatmap.push_back({ 99.24f, DIR_RIGHT});
    mBeatmap.push_back({ 99.61f, DIR_UP   });
    mBeatmap.push_back({ 99.61f, DIR_LEFT });
    mBeatmap.push_back({ 99.96f, DIR_DOWN });
    mBeatmap.push_back({100.32f, DIR_LEFT });
    mBeatmap.push_back({100.32f, DIR_DOWN });
    mBeatmap.push_back({100.67f, DIR_RIGHT});
    mBeatmap.push_back({101.04f, DIR_LEFT });
    mBeatmap.push_back({101.04f, DIR_UP   });
    mBeatmap.push_back({101.39f, DIR_DOWN });
    mBeatmap.push_back({101.74f, DIR_UP   });
    mBeatmap.push_back({102.10f, DIR_RIGHT});
    mBeatmap.push_back({102.46f, DIR_UP   });
    mBeatmap.push_back({102.46f, DIR_LEFT });
    mBeatmap.push_back({102.82f, DIR_DOWN });
    mBeatmap.push_back({103.17f, DIR_LEFT });
    mBeatmap.push_back({103.17f, DIR_DOWN });
    mBeatmap.push_back({103.53f, DIR_RIGHT});
    mBeatmap.push_back({103.89f, DIR_LEFT });
    mBeatmap.push_back({103.89f, DIR_UP   });
    mBeatmap.push_back({104.25f, DIR_DOWN });
    mBeatmap.push_back({104.61f, DIR_UP   });
    mBeatmap.push_back({104.95f, DIR_RIGHT});
    mBeatmap.push_back({105.31f, DIR_UP   });
    mBeatmap.push_back({105.31f, DIR_LEFT });
    mBeatmap.push_back({105.67f, DIR_DOWN });
    mBeatmap.push_back({106.03f, DIR_LEFT });
    mBeatmap.push_back({106.03f, DIR_DOWN });
    mBeatmap.push_back({106.37f, DIR_RIGHT});
    mBeatmap.push_back({106.74f, DIR_LEFT });
    mBeatmap.push_back({106.74f, DIR_UP   });
    mBeatmap.push_back({107.10f, DIR_DOWN });
    mBeatmap.push_back({107.46f, DIR_UP   });
    mBeatmap.push_back({107.82f, DIR_RIGHT});
    mBeatmap.push_back({108.17f, DIR_UP   });
    mBeatmap.push_back({108.17f, DIR_LEFT });

    std::sort(mBeatmap.begin(), mBeatmap.end(),
              [](const BeatNote &a, const BeatNote &b){ return a.time < b.time; });
}

void DanceGameScreen::initialise()
{
    mGameState.nextSceneID = 0;
    mGameState.score = mScore = mCombo = mMaxCombo = 0;
    mPerfects = mGoods = mMisses = 0;
    mTime = 0.0f;
    mBeatIndex = 0;
    mPhase = PHASE_PLAYING;
    mNotes.clear();

    for (int i = 0; i < 4; i++)
    {
        mHitFX[i] = {};
        mWasDown[i] = false;
    }

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    mBgTile = LoadTexture("assets/background.png");
    mBoxTex = LoadTexture("assets/background_box.png");
    SetTextureFilter(mBgTile, TEXTURE_FILTER_POINT);
    SetTextureFilter(mBoxTex, TEXTURE_FILTER_POINT);

    Image di = LoadImage("assets/dancer.png");
    ImageColorReplace(&di, BLACK, BLANK);
    mDancer.texture = LoadTextureFromImage(di);
    UnloadImage(di);

    SetTextureFilter(mDancer.texture, TEXTURE_FILTER_POINT);
    mDancer.frameCount = 4;
    mDancer.currentFrame = 0;
    mDancer.frameTimer = 0;
    mDancer.frameSpeed = 8.0f;
    mDancer.drawSize = 140.0f;

    mDancer2 = mDancer;
    mDancer2.currentFrame = 2;

    mLaneBg = LoadTexture("assets/inner-game-bg.png");
    SetTextureFilter(mLaneBg, TEXTURE_FILTER_POINT);

    Image ai = LoadImage("assets/arrowwhite.png");
    ImageFormat(&ai, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    ImageColorReplace(&ai, BLACK, BLANK);
    mArrowTex = LoadTextureFromImage(ai);
    UnloadImage(ai);
    SetTextureFilter(mArrowTex, TEXTURE_FILTER_POINT);

    mHighlightTex = {0};

    float hudH    = 55.0f;
    float padding = 20.0f;

    float maxPanelW = sw * 0.50f - padding * 2.0f;
    float maxPanelH = sh - hudH - padding * 2.0f;

    mScale  = std::min(maxPanelW / (float)mLaneBg.width,
                       maxPanelH / (float)mLaneBg.height);

    mPanelW = mLaneBg.width  * mScale;
    mPanelH = mLaneBg.height * mScale;

    mPanelX = (sw - mPanelW) / 2.0f;
    mPanelY = hudH + padding + (maxPanelH - mPanelH) / 2.0f;

    float laneProps[4] = {
        0.235f,  // LEFT
        0.385f,  // DOWN
        0.630f,  // UP
        0.780f   // RIGHT
    };

    for (int i = 0; i < 4; i++)
        mLaneCX[i] = mPanelX + laneProps[i] * mPanelW;

    mArrowSz = mPanelW * 0.38f;
    mTargetY = mPanelY + 0.842f * mPanelH;
    mSpawnY  = mPanelY + 0.0237f * mPanelH;

    // How long a note takes to travel from spawn to target
    mTravelTime = (mTargetY - mSpawnY) / mNoteSpeed;

    float dancerY = hudH + (sh - hudH) * 0.50f;
    mDancer.position  = { mPanelX - 180.0f,           dancerY };
    mDancer2.position = { mPanelX + mPanelW + 180.0f, dancerY };

    buildBeatmap();

    mGameState.bgm = LoadMusicStream("assets/weare.wav");
    SetMusicVolume(mGameState.bgm, 0.7f);
    PlayMusicStream(mGameState.bgm);
}

// ─────────────────────────────────────────────────────────────────
// spawnNote — now takes a direction instead of picking randomly
// ─────────────────────────────────────────────────────────────────
void DanceGameScreen::spawnNote(ArrowDir dir)
{
    Note n;
    n.dir      = dir;
    n.y        = mSpawnY;
    n.active   = true;
    n.hitFlash = 0.0f;
    mNotes.push_back(n);
}

// ─────────────────────────────────────────────────────────────────
// tryHit — same logic, now also tracks stats
// ─────────────────────────────────────────────────────────────────
void DanceGameScreen::tryHit(ArrowDir dir)
{
    float bestDist = 9999.0f;
    Note *best = nullptr;

    for (auto &n : mNotes)
    {
        if (!n.active || n.dir != dir) continue;

        float d = fabsf(n.y - mTargetY);
        if (d < bestDist)
        {
            bestDist = d;
            best = &n;
        }
    }

    HitRating r = RATE_MISS;

    if (best && bestDist < mArrowSz * 0.38f)
    {
        r = RATE_PERFECT; mPerfects++;
        best->active = false;
        best->hitFlash = 0.15f;
        mCombo++;
        mScore += 300 + mCombo * 10;
        mDancer.frameSpeed = 18.0f;
    }
    else if (best && bestDist < mArrowSz * 0.70f)
    {
        r = RATE_GOOD; mGoods++;
        best->active = false;
        best->hitFlash = 0.15f;
        mCombo++;
        mScore += 100 + mCombo * 5;
        mDancer.frameSpeed = 13.0f;
    }
    else
    {
        mMisses++;
        mCombo = 0;
    }

    if (mCombo > mMaxCombo) mMaxCombo = mCombo;
    mGameState.score = mScore;
    mHitFX[(int)dir] = { r, 0.65f, 0.0f };
}

// ─────────────────────────────────────────────────────────────────
// update — identical logic, random timer replaced with beatmap
// ─────────────────────────────────────────────────────────────────
void DanceGameScreen::update(float deltaTime)
{
    mTime += deltaTime;

    // Results screen: Return/Enter/Space go back to menu.
    // Do NOT update music here because the song is stopped when results begin.
    if (mPhase == PHASE_RESULTS)
    {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) || IsKeyPressed(KEY_SPACE))
        {
            StopMusicStream(mGameState.bgm);
            mGameState.nextSceneID = -2;
        }
        return;
    }

    UpdateMusicStream(mGameState.bgm);

    if (mDancer.frameSpeed > 8.0f)
        mDancer.frameSpeed = std::max(8.0f, mDancer.frameSpeed - deltaTime * 18.0f);

    mDancer2.frameSpeed = mDancer.frameSpeed;
    updateDancer(deltaTime);

    mDancer2.frameTimer += deltaTime;
    if (mDancer2.frameTimer >= 1.0f / mDancer2.frameSpeed)
    {
        mDancer2.frameTimer = 0.0f;
        mDancer2.currentFrame = (mDancer2.currentFrame + 1) % mDancer2.frameCount;
    }

    // ── Beatmap spawning (replaces random timer) ──────────────────
    float songTime = GetMusicTimePlayed(mGameState.bgm);

    while (mBeatIndex < (int)mBeatmap.size())
    {
        float spawnAt = mBeatmap[mBeatIndex].time - mTravelTime;
        if (songTime >= spawnAt)
        {
            spawnNote(mBeatmap[mBeatIndex].dir);
            mBeatIndex++;
        }
        else break;
    }

    // ── Move notes (identical to yours) ──────────────────────────
    for (auto &n : mNotes)
    {
        if (!n.active) continue;

        n.y += mNoteSpeed * deltaTime;

        if (n.hitFlash > 0)
            n.hitFlash -= deltaTime;

        if (n.y > mTargetY + mArrowSz * 0.85f)
        {
            n.active = false;
            mCombo   = 0; mMisses++;
            mHitFX[(int)n.dir] = { RATE_MISS, 0.55f, 0.0f };
        }
    }

    mNotes.erase(
        std::remove_if(mNotes.begin(), mNotes.end(),
            [](const Note &n) { return !n.active; }),
        mNotes.end());

    for (int i = 0; i < 4; i++)
    {
        if (mHitFX[i].timer > 0)
        {
            mHitFX[i].timer -= deltaTime;
            mHitFX[i].rise  -= deltaTime * 40.0f;
        }
    }

    // ── Input (identical to yours) ────────────────────────────────
    KeyboardKey k1[4] = { KEY_LEFT, KEY_DOWN, KEY_UP, KEY_RIGHT };
    KeyboardKey k2[4] = { KEY_A, KEY_S, KEY_W, KEY_D };

    for (int i = 0; i < 4; i++)
    {
        bool dn = IsKeyDown(k1[i]) || IsKeyDown(k2[i]);
        if (dn && !mWasDown[i]) tryHit((ArrowDir)i);
        mWasDown[i] = dn;
    }

    if (IsKeyPressed(KEY_ESCAPE)) mGameState.nextSceneID = -2;

    // ── Song ended → show results ─────────────────────────────────
    float songLen = GetMusicTimeLength(mGameState.bgm);
    if (songTime >= songLen - 0.5f &&
        mBeatIndex >= (int)mBeatmap.size() &&
        mNotes.empty())
    {
        StopMusicStream(mGameState.bgm);
        mPhase = PHASE_RESULTS;
    }
}

// ═════════════════════════════════════════════════════════════════
// EVERYTHING BELOW IS IDENTICAL TO YOUR ORIGINAL — NOT TOUCHED
// ═════════════════════════════════════════════════════════════════

void DanceGameScreen::drawDancerBox2() const
{
    if (mBoxTex.id != 0)
    {
        float bw = (float)mBoxTex.width * 3.0f;
        float bh = (float)mBoxTex.height * 3.0f;

        Rectangle bSrc = { 0, 0, (float)mBoxTex.width, (float)mBoxTex.height };
        Rectangle bDst = {
            mDancer2.position.x - bw / 2.0f,
            mDancer2.position.y - bh / 2.0f,
            bw,
            bh
        };

        DrawTexturePro(mBoxTex, bSrc, bDst, {0, 0}, 0.0f, WHITE);
    }

    if (mDancer2.texture.id == 0) return;

    static const int FX[4] = {  32, 100, 200, 300 };
    static const int FY[4] = {   0,   8,   8,   0 };
    static const int FW[4] = {  68, 100, 100,  59 };
    static const int FH[4] = {  96,  87,  86,  94 };

    int f = mDancer2.currentFrame;

    Rectangle src = { (float)FX[f], (float)FY[f], (float)FW[f], (float)FH[f] };

    float scale = mDancer2.drawSize / (float)std::max(FW[f], FH[f]);
    float dw = FW[f] * scale;
    float dh = FH[f] * scale;

    Rectangle dst = {
        mDancer2.position.x - dw / 2.0f,
        mDancer2.position.y - dh / 2.0f,
        dw,
        dh
    };

    DrawTexturePro(mDancer2.texture, src, dst, {0, 0}, 0.0f, WHITE);
}

void DanceGameScreen::drawPanel() const
{
    Rectangle src = { 0, 0, (float)mLaneBg.width, (float)mLaneBg.height };
    Rectangle dst = { mPanelX, mPanelY, mPanelW, mPanelH };
    DrawTexturePro(mLaneBg, src, dst, {0, 0}, 0.0f, WHITE);
}

void DanceGameScreen::drawTargets() const
{
    float rotations[4] = { -90.0f, 180.0f, 0.0f, 90.0f };

    KeyboardKey k1[4] = { KEY_LEFT, KEY_DOWN, KEY_UP, KEY_RIGHT };
    KeyboardKey k2[4] = { KEY_A, KEY_S, KEY_W, KEY_D };

    float offsetX[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    float offsetY[4] = { 12.0f, 2.0f, 34.0f, 25.0f };

    float hs = (float)mArrowTex.width;

    for (int i = 0; i < 4; i++)
    {
        float sz = mArrowSz;

        Rectangle src = { 0, 0, hs, hs };
        Rectangle dst = {
            mLaneCX[i] + offsetX[i],
            mTargetY + offsetY[i],
            sz,
            sz
        };

        Vector2 ori = { sz / 2.0f, sz / 2.0f };

        bool held = IsKeyDown(k1[i]) || IsKeyDown(k2[i]);
        Color col = held ? WHITE : Color{ 180, 180, 180, 120 };

        DrawTexturePro(mArrowTex, src, dst, ori, rotations[i], col);
    }
}

void DanceGameScreen::drawNotes() const
{
    float rotations[4] = { -90.0f, 180.0f, 0.0f, 90.0f };

    Color tints[4] = {
        { 255,  40,  40, 255 },
        {  30, 180, 255, 255 },
        { 255, 210,  40, 255 },
        {  30, 210,  80, 255 }
    };

    float offsetX[4] = { 0.0f,   0.0f, 0.0f,   0.0f  };
    float offsetY[4] = { 0.0f,  -8.0f, 6.0f, -25.0f  };

    float hs = (float)mArrowTex.width;

    for (const auto &n : mNotes)
    {
        if (!n.active) continue;
        if (n.y < mSpawnY - mArrowSz || n.y > mTargetY + mArrowSz) continue;

        int i = (int)n.dir;
        float sz = mArrowSz;

        Rectangle src = { 0, 0, hs, hs };
        Rectangle dst = {
            mLaneCX[i] + offsetX[i],
            n.y + offsetY[i],
            sz,
            sz
        };

        Vector2 ori = { sz / 2.0f, sz / 2.0f };

        Color c = (n.hitFlash > 0) ? WHITE : tints[i];

        DrawTexturePro(mArrowTex, src, dst, ori, rotations[i], c);
    }
}

void DanceGameScreen::drawFX() const
{
    for (int i = 0; i < 4; i++)
    {
        if (mHitFX[i].timer <= 0) continue;

        float alpha = std::min(1.0f, mHitFX[i].timer / 0.45f);
        unsigned char a = (unsigned char)(alpha * 255);

        const char *label;
        Color col;

        switch (mHitFX[i].rating)
        {
            case RATE_PERFECT:
                label = "PERFECT";
                col = {255, 220, 50, a};
                break;
            case RATE_GOOD:
                label = "GOOD";
                col = {100, 255, 100, a};
                break;
            case RATE_MISS:
                label = "MISS";
                col = {255, 70, 70, a};
                break;
            default:
                continue;
        }

        int sz = 18;
        int tw = MeasureText(label, sz);
        float fy = mTargetY - 55.0f + mHitFX[i].rise;

        DrawText(label, (int)(mLaneCX[i] - tw / 2.0f), (int)fy, sz, col);
    }
}

void DanceGameScreen::drawHUD() const
{
    DrawRectangle(0, 0, GetScreenWidth(), 55, {0, 0, 0, 190});
    DrawLine(0, 55, GetScreenWidth(), 55, {180, 230, 255, 100});

    DrawText("DANCE RUSH", 20, 14, 26, WHITE);

    char buf[32];
    sprintf(buf, "SCORE: %06d", mScore);

    int sw = MeasureText(buf, 22);
    DrawText(buf, GetScreenWidth() - sw - 20, 16, 22, {255, 220, 80, 255});

    if (mCombo >= 2)
    {
        char cb[32];
        sprintf(cb, "%dx COMBO", mCombo);

        DrawText(cb, 16, GetScreenHeight() - 62, 22, {255, 220, 50, 230});
    }

    DrawText("q or esc: quit", 16, GetScreenHeight() - 24, 13, {200, 220, 255, 110});
}

// ── NEW: results screen overlay ───────────────────────────────────
void DanceGameScreen::drawResults() const
{
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    DrawRectangle(0, 0, sw, sh, {0, 0, 0, 210});

    const char *title = (mDifficulty == DIFFICULTY_EASY) ? "EASY RESULTS" : "HARD RESULTS";
    int tw = MeasureText(title, 64);
    DrawText(title, sw/2 - tw/2, sh/2 - 190, 64, {255, 220, 50, 255});

    DrawLine(sw/2 - 220, sh/2 - 110, sw/2 + 220, sh/2 - 110, {255, 255, 255, 60});

    char buf[64];
    int lineH  = 48;
    int startY = sh/2 - 90;

    snprintf(buf, sizeof(buf), "SCORE      %06d", mScore);
    DrawText(buf, sw/2 - MeasureText(buf, 28)/2, startY,           28, WHITE);

    snprintf(buf, sizeof(buf), "MAX COMBO  %d",   mMaxCombo);
    DrawText(buf, sw/2 - MeasureText(buf, 28)/2, startY + lineH,   28, {100, 255, 200, 255});

    snprintf(buf, sizeof(buf), "PERFECT    %d",   mPerfects);
    DrawText(buf, sw/2 - MeasureText(buf, 28)/2, startY + lineH*2, 28, {255, 220, 50, 255});

    snprintf(buf, sizeof(buf), "GOOD       %d",   mGoods);
    DrawText(buf, sw/2 - MeasureText(buf, 28)/2, startY + lineH*3, 28, {100, 255, 100, 255});

    snprintf(buf, sizeof(buf), "MISS       %d",   mMisses);
    DrawText(buf, sw/2 - MeasureText(buf, 28)/2, startY + lineH*4, 28, {255, 80, 80, 255});

    // Grade
    int   total = mPerfects + mGoods + mMisses;
    float pct   = total > 0 ? (float)(mPerfects + mGoods) / total : 0.0f;
    const char *grade; Color gc;
    if      (pct >= 0.95f && mMisses == 0) { grade="S"; gc={255,220, 50,255}; }
    else if (pct >= 0.90f)                 { grade="A"; gc={100,255,100,255}; }
    else if (pct >= 0.75f)                 { grade="B"; gc={100,200,255,255}; }
    else if (pct >= 0.60f)                 { grade="C"; gc={200,200,100,255}; }
    else                                   { grade="D"; gc={255,100,100,255}; }

    int gw = MeasureText(grade, 100);
    DrawText(grade, sw/2 - gw/2, startY + lineH*5 + 5, 100, gc);

    DrawLine(sw/2 - 220, sh/2 + 225, sw/2 + 220, sh/2 + 225, {255, 255, 255, 60});

    if ((int)(mTime * 2) % 2 == 0)
    {
        const char *prompt = "PRESS ENTER TO RETURN TO MENU";
        int pw = MeasureText(prompt, 20);
        DrawText(prompt, sw/2 - pw/2, sh/2 + 238, 20, {255, 255, 255, 200});
    }
}

// ─────────────────────────────────────────────────────────────────
// render — identical, + drawResults overlay at end
// ─────────────────────────────────────────────────────────────────
void DanceGameScreen::render()
{
    drawTiledBackground(GetScreenWidth(), GetScreenHeight());

    DrawRectangleGradientV(
        0, 0, GetScreenWidth(), GetScreenHeight(),
        {0, 0, 0, 30}, {0, 0, 0, 90});

    drawDancerBox();
    drawDancerBox2();
    drawPanel();
    drawTargets();
    drawNotes();
    drawFX();
    drawHUD();

    if (mPhase == PHASE_RESULTS)
        drawResults();
}

// ─────────────────────────────────────────────────────────────────
// shutdown — identical to yours
// ─────────────────────────────────────────────────────────────────
void DanceGameScreen::shutdown()
{
    if (mDancer.texture.id != 0) UnloadTexture(mDancer.texture);
    mDancer2.texture.id = 0;

    if (mBoxTex.id      != 0) UnloadTexture(mBoxTex);
    if (mBgTile.id      != 0) UnloadTexture(mBgTile);
    if (mLaneBg.id      != 0) UnloadTexture(mLaneBg);
    if (mArrowTex.id    != 0) UnloadTexture(mArrowTex);
    if (mHighlightTex.id!= 0) UnloadTexture(mHighlightTex);

    UnloadMusicStream(mGameState.bgm);

    mDancer.texture.id = 0;
    mBoxTex.id = 0;
    mBgTile.id = 0;
    mLaneBg.id = 0;
    mArrowTex.id = 0;
    mHighlightTex.id = 0;
}
