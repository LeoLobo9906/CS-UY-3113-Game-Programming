#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>

enum appStatus {TERMINATED, RUNNING };

appStatus gAppStatus = RUNNING;

void initialise();
void processInput();
void update();
void render();
void shutdown();

void initialise() {
    InitWindow(
        000, //Width in pixels
        450, //Height in pixels
        "Hello Raylb"
    );

    SetTargetFPS(60);

    // Initialise the camera
    // Initialise any game objects (e.g. player, enemies, map, etc.)
}

void processInput() {
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {

}

void render() {
    BeginDrawing();

    clearBackground(RAYWHITE);

#include <stdio.h>
#include "raylib.h"

enum appStatus {TERMINATED, RUNNING };

appStatus gAppStatus = RUNNING;

void initialise();
void processInput();
void update();
void render();
void shutdown();

void initialise() {
    InitWindow(
        000, //Width in pixels
        450, //Height in pixels
        "Hello Raylb"
    );

    SetTargetFPS(60);

    // Initialise the camera
    // Initialise any game objects (e.g. player, enemies, map, etc.)
}

void processInput() {
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {

}

void render() {
    BeginDrawing();

    clearBackground(RAYWHITE);


    EndDrawing();
}

void shutdown() {
    CloseWIndow();
}

int main(void){
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();
    return 0;
}

    EndDrawing();
}

void shutdown() {
    CloseWIndow();
}

int main(void){
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();
    return 0;
}
