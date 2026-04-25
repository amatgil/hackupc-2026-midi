#include <cstdlib>
#include <stdio.h>
#include "raylib.h"
#include "assert.h"
#include "tests.hh"

#include "window_midi_editor.hh"
#include "window_midi_player.hh"

#include "sheet.hh"
#include "parsing.hh"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

void run_tests() {
  parse_header_from_file();
}  

int main(int argc, char* argv[])
{
  if (argc == 2 && argv[1][0] == 't') {
      run_tests();
      return 0;
  }
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);   
  if (argc == 3) {
    int wdth = atoi(argv[1]);
    int hght = atoi(argv[2]);

    InitWindow(wdth, hght, "Midi");
  } else {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Midi");
  }
    //ToggleFullscreen();

    InitAudioDevice();
    SetTargetFPS(60);

    initialize_midi_player();

    Sheet sheet = 
    {
        {0,1,2,3,4}, // timestamps_start
        {1,1,1,1,1}, // durations
        {0,13,28,39,51}, // pitch
		{100,100,100,100,100}, // attack_velocities
		5 // total_duration
    };
	play_midi(sheet);

    float deltaTime = 0;
    while (!WindowShouldClose())
    {
        deltaTime = GetFrameTime();

        BeginDrawing();
        
		update_midi_playback(deltaTime);
        draw_midi_player_screen();

        drawSoundTimeline(sheet);

        EndDrawing();

        // UPDATE APP
    }

    unload_midi_player();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}


