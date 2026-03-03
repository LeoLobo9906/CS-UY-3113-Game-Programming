#ifndef CS3113_H
#define CS3113_H

#include "raylib.h"
#include <stdio.h>

enum AppStatus { TERMINATED, RUNNING };

Color ColorFromHex(const char *hex);

// Vector helpers (used in lecture 4 exercises)
float GetLength(Vector2 *v);
void  Normalise(Vector2 *v);

#endif // CS3113_H