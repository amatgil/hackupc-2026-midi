#include "window_midi_player.hh"

Camera2D _cam = { 0 };

Sheet playing_sheet;
float playing_time = 0.0f;

void update_midi_playback(const float deltaTime)
{
	playing_time += deltaTime;
}

void play_midi(const Sheet& sheet) 
{
	playing_sheet = sheet;
	playing_time = 0.0f;
}

void draw_midi_player_screen()
{
    _cam.target = { 0, 0 };
    //_cam.offset = { width / 2.0f, height / 2.0f }; // Screen center
    _cam.rotation = 0.0f;
    //_cam.zoom = (float)height / MAP_COORD_SIZE;

    BeginMode2D(_cam);
    ClearBackground(COLOR_BACKGROUND);

    EndMode2D();
}