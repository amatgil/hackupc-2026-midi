#include "window_midi_player.hh"

Camera2D _cam = { 0 };

float playing_time = 0.0f;

int notes_count = 0;
midi_player_note* notes = nullptr;

Texture2D piano_texture;

void update_midi_playback(const float deltaTime)
{
	playing_time += deltaTime;
}

void play_midi(const Sheet& sheet) 
{
    piano_texture = LoadTexture("../assets/ui/UI_MidiPlayer_Piano.png");

	playing_time = 0.0f;

	notes_count = sheet.pitch.size();
	if (notes != nullptr) free(notes);

    notes = (midi_player_note*)malloc(sizeof(midi_player_note) * notes_count);
	for (int i = 0; i < notes_count; ++i)
    {
        float width = 1.0f;
        float height = (float)sheet.durations[i] * VERTICAL_SCALE;
        float x = (float)sheet.pitch[i] - 44; // TODO: posar de -44 a 43
        float y = -(float)sheet.timestamps_start[i] * VERTICAL_SCALE - height;

        notes[i].position = { x, y };
        notes[i].size = { width, height };

		printf("Note %d: position=(%f, %f), size=(%f, %f)\n", i, x, y, width, height);
    }
}

void draw_midi_player_screen()
{
	_cam.target = { 0, -playing_time * VERTICAL_SCALE };
	_cam.offset = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	_cam.rotation = 0.0f;
	_cam.zoom = (float)GetScreenWidth() / 88.0f;

    BeginMode2D(_cam);
    ClearBackground(COLOR_BACKGROUND);

    for (int i = 0; i < notes_count; ++i)
    {
        DrawRectangleV(notes[i].position, notes[i].size, COLOR_NOTE);
	}

    EndMode2D();

	Rectangle dest = { 0, GetScreenHeight() / 2.0f, (float)GetScreenWidth(), GetScreenHeight() / 2.0f};
	Vector2 origin = { 1.0f / 2.0f, 1.0f / 2.0f };

	DrawTexturePro
	(piano_texture,
		{
			0,
			0,
			900,
			450
		},
		dest, origin, 0, WHITE
	);
}