#ifndef WINDOW_MIDI_PLAYER_HH
#define WINDOW_MIDI_PLAYER_HH

#include <cstdlib>
#include <stdio.h>
#include <cmath>
#include <unordered_map>

#include "raylib.h"
#include "sheet.hh"
#include "note_player.hh"

#define COLOR_BACKGROUND (Color){ 88, 8, 94, 255 }
#define COLOR_NOTE (Color){ 212, 16, 230, 255 }

#define VERTICAL_SCALE 10.0f

#define PIANO_VERTICAL_WINDOW_PROPORTION 1.0f/5.0f

#define INITIAL_DELAY 5.0f

#define BACKGROUND_ROTATION -15.0f
#define BACKGROUND_SPEED 5.0f
#define BACKGROUND_TEXTURE_SIZE 960.0f
#define BACKGROUND_SCALE 1.0f

const unordered_map<unsigned int, float> pitch_to_position = {
	{ 0, -26 }, 
	{ 1, -25.5f }, 
	{ 2, -25 }, 
	{ 3, -24 }, 
	{ 4, -23.5f }, 
	{ 5, -23 }, 
	{ 6, -22.5f }, 
	{ 7, -22 }, 
	{ 8, -21 }, 
	{ 9, -20.5f }, 
	{ 10, -20 }, 
	{ 11, -19.5f }, 
	{ 12, -19 }, 
	{ 13, -18.5f }, 
	{ 14, -18 }, 
	{ 15, -17 }, 
	{ 16, -16.5f }, 
	{ 17, -16 }, 
	{ 18, -15.5f }, 
	{ 19, -15 }, 
	{ 20, -14 }, 
	{ 21, -13.5f }, 
	{ 22, -13 }, 
	{ 23, -12.5f }, 
	{ 24, -12 }, 
	{ 25, -11.5f }, 
	{ 26, -11 }, 
	{ 27, -10 }, 
	{ 28, -9.5f }, 
	{ 29, -9 }, 
	{ 30, -8.5f }, 
	{ 31, -8 }, 
	{ 32, -7 }, 
	{ 33, -6.5f }, 
	{ 34, -6 }, 
	{ 35, -5.5f }, 
	{ 36, -5 }, 
	{ 37, -4.5f }, 
	{ 38, -4 }, 
	{ 39, -3 }, 
	{ 40, -2.5f }, 
	{ 41, -2 }, 
	{ 42, -1.5f }, 
	{ 43, -1 }, 
	{ 44, 0 }, 
	{ 45, 0.5f }, 
	{ 46, 1 }, 
	{ 47, 1.5f }, 
	{ 48, 2 }, 
	{ 49, 2.5f }, 
	{ 50, 3 }, 
	{ 51, 4 }, 
	{ 52, 4.5f }, 
	{ 53, 5 }, 
	{ 54, 5.5f }, 
	{ 55, 6 }, 
	{ 56, 7 }, 
	{ 57, 7.5f }, 
	{ 58, 8 }, 
	{ 59, 8.5f }, 
	{ 60, 9 }, 
	{ 61, 9.5f }, 
	{ 62, 10 }, 
	{ 63, 11 }, 
	{ 64, 11.5f }, 
	{ 65, 12 }, 
	{ 66, 12.5f }, 
	{ 67, 13 }, 
	{ 68, 14 }, 
	{ 69, 14.5f }, 
	{ 70, 15 }, 
	{ 71, 15.5f }, 
	{ 72, 16 }, 
	{ 73, 16.5f }, 
	{ 74, 17 }, 
	{ 75, 18 }, 
	{ 76, 18.5f }, 
	{ 77, 19 }, 
	{ 78, 19.5f }, 
	{ 79, 20 }, 
	{ 80, 21 }, 
	{ 81, 21.5f }, 
	{ 82, 22 }, 
	{ 83, 22.5f }, 
	{ 84, 23 }, 
	{ 85, 23.5f }, 
	{ 86, 24 }, 
	{ 87, 25 },
};

typedef struct {
	Vector2 position;
	Vector2 size;
	int pitch;
	bool played;
} midi_player_note;

void initialize_midi_player();
void unload_midi_player();

void play_midi(const Sheet& sheet);

void update_midi_playback(const float deltaTime);

void draw_midi_player_screen();

#endif