#ifndef WINDOW_MIDI_PLAYER_HH
#define WINDOW_MIDI_PLAYER_HH

#include <cstdlib>
#include <stdio.h>
#include <cmath>

#include "sheet.hh"
#include "note_player.hh"

#include "window_common.hh"

#define VERTICAL_SCALE 10.0f

#define PIANO_VERTICAL_WINDOW_PROPORTION 1.0f/5.0f

#define INITIAL_DELAY 5.0f

#define BACKGROUND_ROTATION -15.0f
#define BACKGROUND_SPEED 5.0f
#define BACKGROUND_TEXTURE_SIZE 960.0f
#define BACKGROUND_SCALE 1.0f

typedef struct {
	Vector2 position;
	Vector2 size;
	int pitch;
	bool played;
	bool is_flat;
} midi_player_note;

void initialize_midi_player();
void unload_midi_player();

void play_midi(const Sheet& sheet);

void update_midi_playback(const float deltaTime);

void draw_midi_player_screen();

#endif