#include <cstdlib>
#include <stdio.h>
#include "raylib.h"

#include "sheet.cc"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

int main(int argc, char* argv[])
{
    if (argc == 3)
    {
        int wdth = atoi(argv[1]);
        int hght = atoi(argv[2]);

        InitWindow(wdth, hght, "Midi");
    }
    else
    {
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Midi");
    }

    SetExitKey(KEY_NULL);

    InitAudioDevice();
    SetTargetFPS(60);

    float deltaTime = 0;
    while (!WindowShouldClose())
    {
        deltaTime = GetFrameTime();

        // UPDATE APP
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
