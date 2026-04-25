#include "window_common.hh"

void load_common_gui_assets()
{
	piano_texture = LoadTexture("assets/piano.png");
}

void unload_common_gui_assets()
{
	UnloadTexture(piano_texture);
}