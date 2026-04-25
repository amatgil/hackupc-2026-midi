#ifndef WINDOW_MIDI_PLAYER_HH
#define WINDOW_MIDI_PLAYER_HH

#include "sheet.hh"
#include "raylib.h"

#define COLOR_BACKGROUND (Color){ 88, 8, 94, 255 }

void play_midi(const Sheet& sheet);

void draw_midi_player_screen();

#endif