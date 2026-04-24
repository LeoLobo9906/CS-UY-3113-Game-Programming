#ifndef DANCE_CS3113_H
#define DANCE_CS3113_H

#define LOG(argument) std::cout << argument << '\n'

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>

enum AppStatus { TERMINATED, RUNNING };

// ── Colour helper ─────────────────────────────────────────────────
Color ColorFromHex(const char *hex);

// ── Camera helper ─────────────────────────────────────────────────
void panCamera(Camera2D *camera, const Vector2 *targetPosition);

#endif // DANCE_CS3113_H
