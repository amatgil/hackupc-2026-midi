#include <cstdlib>
#include <stdio.h>
#include "raylib.h"
#include "assert.h"
#include "tests.hh"
#include "fftw3.h"
#include "app_mode.hh"

#include "window_midi_editor.hh"
#include "window_midi_player.hh"

#include "sheet.hh"
#include "parsing.hh"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

void run_tests() {
  //parse_header_from_file();
  //test_FFT_samples();
  test_FFT_samples_calling();
  //test_variable_length_quantity();
  test_full_parse();
}  

Sheet generate_full_piano_sheet()
{
    Sheet sheet;

    sheet.total_duration = 88.0f;

    for (int i = 0; i < 88; ++i)
    {
        sheet.timestamps_start.push_back((float)i/2.);
        sheet.durations.push_back(0.5f);
        sheet.pitch.push_back(i);
        sheet.attack_velocities.push_back(100);
    }

    return sheet;
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

    Sheet sheet = read_midi_file("../assets/testfiles/moonlight.mid", 120);


	fftw_plan plan{};

	AppMode app_mode = Edit;

	initEditor();

    float deltaTime = 0;
    while (!WindowShouldClose())
    {
        deltaTime = GetFrameTime();

        BeginDrawing();

        if(app_mode == Play) {
            update_midi_playback(deltaTime);
            draw_midi_player_screen();
        } else {
            drawSoundTimeline(sheet);
        }

        if(IsKeyReleased(KEY_P)) {
            if(app_mode == Play) {
                app_mode = Edit;
            } else if(app_mode == Edit) {
                play_midi(sheet);
                app_mode = Play;
            }
        }

        EndDrawing();

        // UPDATE APP
    }

    unload_midi_player();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
