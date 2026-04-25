#include <cstdlib>
#include <stdio.h>
#include "raylib.h"
#include "assert.h"

#include "window_midi_editor.hh"
#include "window_midi_player.hh"

#include "sheet.hh"
#include "parsing.hh"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

int main_fake(int argc, char* argv[])
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

        draw_midi_player_screen();

        //drawSoundTimeline();


        EndDrawing();

        // UPDATE APP
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}


int main() {
  char s[] = {0x4d, 0x54, 0x68, 0x64,
              0x00, 0x00, 0x00, 0x06,
              0x00, 0x01,
              0x00, 0x02
  };

  Header h = parse_headers(s, 12);
  assert(h.length == 6);
  assert(h.format == 1);
  assert(h.division == 2);
  printf("Tot ok!\n");
}
