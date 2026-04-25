#ifndef WINDOW_MIDI_PLAYER_HH
#define WINDOW_MIDI_PLAYER_HH

#include <cstdlib>
#include <stdio.h>

#include "sheet.hh"
#include "raylib.h"

#define COLOR_BACKGROUND (Color){ 88, 8, 94, 255 }
#define COLOR_NOTE (Color){ 212, 16, 230, 255 }

#define VERTICAL_SCALE 10.0f

typedef struct {
	Vector2 position;
	Vector2 size;
} midi_player_note;

void play_midi(const Sheet& sheet);

void update_midi_playback(const float deltaTime);

void draw_midi_player_screen();

#endif