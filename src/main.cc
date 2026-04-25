#include <cstdlib>
#include <stdio.h>
#include "raylib.h"

#include "window_midi_editor.hh"

#include "sheet.hh"

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

    ToggleFullscreen();

    InitAudioDevice();
    SetTargetFPS(60);

    Sheet sheet;

    float deltaTime = 0;
    while (!WindowShouldClose())
    {
        deltaTime = GetFrameTime();

        BeginDrawing();

        draw_midi_player_screen(sheet);

        drawSoundTimeline(sheet);


        EndDrawing();

        // UPDATE APP
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
