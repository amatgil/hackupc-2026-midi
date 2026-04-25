#include "window_common.hh"

void load_common_gui_assets()
{
	piano_texture = LoadTexture("../assets/ui/UI_MidiPlayer_Piano.png");
	piano_texture_vertical = LoadTexture("../assets/ui/UI_MidiPlayer_Piano_Vertical.png");
}

void unload_common_gui_assets()
{
	UnloadTexture(piano_texture);
	UnloadTexture(piano_texture_vertical);
}