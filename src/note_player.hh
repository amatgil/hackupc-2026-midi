#ifndef NOTE_PLAYER_HH
#define NOTE_PLAYER_HH

#include "raylib.h"

void load_note_sounds();

void unload_note_sounds();

void play_note_sound(unsigned int pitch, unsigned int velocity);

void stop_note_sound(unsigned int pitch);

#endif