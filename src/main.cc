#include <cstdlib>
#include <stdio.h>
#include "raylib.h"
#include "assert.h"
#include "tests.hh"

#include "window_midi_editor.hh"
#include "window_midi_player.hh"

#include "sheet.hh"
#include "parsing.hh"
#include "fft.hh"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

void run_tests() {
  // parse_header_from_file();
  Wave la = LoadWave("../assets/samples/Mf A4.wav");
  printf("Got wav\n");
  float* samples = LoadWaveSamples(la);
  int N = la.frameCount * la.channels;
  printf("Got samples\n");
  FFT(samples, N);

  for (unsigned int i = 0; i < N; i++) {
    printf("%i -> %f\n", i, samples[i]);
  }    

  UnloadWaveSamples(samples);  
}  

int main(int argc, char* argv[])
{
  if (argc == 2 && argv[1][0] == 't') {
      run_tests();
      return 0;
  }
  if (argc == 3) {
    int wdth = atoi(argv[1]);
    int hght = atoi(argv[2]);

    InitWindow(wdth, hght, "Midi");
  } else {
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


