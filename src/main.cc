#include <cstdlib>
#include <stdio.h>
#include "raylib.h"

#include "window_midi_editor.hh"
#include "window_midi_player.hh"

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

    Sheet sheet{};
    sheet.attack_velocities = {30, 50, 100, 1, 4}; // 0,1,2,3,4
    sheet.durations = {1.0f, 0.5f, 2.f, 1.5f, 4.f};
    sheet.pitch = {50, 52, 54, 53, 55 };
    sheet.timestamps_start = {0.f, 1.f, 1.5f, 4.f, 5.f};

    initEditor();

    float deltaTime = 0;
    while (!WindowShouldClose())
    {
        deltaTime = GetFrameTime();

        BeginDrawing();

        draw_midi_player_screen();

        drawSoundTimeline(sheet);


        EndDrawing();

        // UPDATE APP
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
