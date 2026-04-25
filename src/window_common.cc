#include "window_common.hh"

Texture2D background_gradient;

void load_common_gui_assets()
{
	piano_texture = LoadTexture("../assets/ui/UI_MidiPlayer_Piano.png");
	piano_texture_vertical = LoadTexture("../assets/ui/UI_MidiPlayer_Piano_Vertical.png");
	background_gradient = LoadTexture("../assets/ui/UI_MidiPlayer_Background_Gradient.png");
}

void unload_common_gui_assets()
{
	UnloadTexture(piano_texture);
	UnloadTexture(piano_texture_vertical);
	UnloadTexture(background_gradient);
}

void draw_background_gradient()
{
	DrawTexturePro(background_gradient,
		{ 0, 0, (float)200, (float)200 },
		{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
		{ 0, 0 },
		0,
		WHITE
	);
}