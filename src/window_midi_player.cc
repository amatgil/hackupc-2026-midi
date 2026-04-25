#include "window_midi_player.hh"

Camera2D _cam = { 0 };

float playing_time = 0.0f;

int notes_count = 0;
midi_player_note* notes = nullptr;

Texture2D piano_texture;

void initialize_midi_player()
{
	piano_texture = LoadTexture("../assets/ui/UI_MidiPlayer_Piano.png");

	load_note_sounds();
}

void unload_midi_player()
{
	UnloadTexture(piano_texture);

	unload_note_sounds();

	if (notes != nullptr) free(notes);
}

void play_midi(const Sheet& sheet)
{
	playing_time = 0.0f;

	notes_count = sheet.pitch.size();
	if (notes != nullptr) free(notes);

	notes = (midi_player_note*)malloc(sizeof(midi_player_note) * notes_count);
	for (int i = 0; i < notes_count; ++i)
	{
		float width = 1.0f;
		float height = (float)sheet.durations[i] * VERTICAL_SCALE;
		float x = (float)sheet.pitch[i] - 26; // TODO: posar de -26 a 25  (Tecles blanques)
		float y = -(float)sheet.timestamps_start[i] * VERTICAL_SCALE - height - INITIAL_DELAY * VERTICAL_SCALE;

		notes[i].position = { x, y };
		notes[i].size = { width, height };

		printf("Note %d: position=(%f, %f), size=(%f, %f)\n", i, x, y, width, height);
	}
}

void update_midi_playback(const float deltaTime)
{
	playing_time += deltaTime;

	for (int i = 0; i < notes_count; ++i)
	{
		notes[i].position.y += VERTICAL_SCALE * deltaTime;
		if (notes[i].position.y >= 0)
		{
			printf("Playing note %d at time %f seconds\n", i, playing_time);
			// Here you can add code to play the sound associated with the note
			// For example, you could use PlaySound() from raylib to play a sound file
			// PlaySound(note_sound);
			notes[i].position.y = 0; // Keep the note at the piano level
		}
	}
}

void draw_piano()
{
	Rectangle dest = { 0, (float)GetScreenHeight() * (1.0f - PIANO_VERTICAL_WINDOW_PROPORTION), (float)GetScreenWidth(), GetScreenHeight() * PIANO_VERTICAL_WINDOW_PROPORTION };
	Vector2 origin = { 1.0f / 2.0f, 1.0f / 2.0f };

	DrawTexturePro
	(piano_texture,
		{
			0,
			0,
			2844,
			350
		},
		dest, origin, 0, WHITE
	);
}

void draw_midi_player_screen()
{
    ClearBackground(COLOR_BACKGROUND);

	draw_piano();

    BeginMode2D(_cam);

	_cam.target = { 0, 0 };
	_cam.offset = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	_cam.rotation = 0.0f;
	_cam.zoom = (float)GetScreenWidth() / 52.0f;

	Vector2 worldCenterPos = GetScreenToWorld2D({ 0, (float)GetScreenHeight() * (1.0f - PIANO_VERTICAL_WINDOW_PROPORTION) }, _cam);
	_cam.target = { 0, -worldCenterPos.y };

    for (int i = 0; i < notes_count; ++i)
    {
        DrawRectangleV(notes[i].position, notes[i].size, COLOR_NOTE);
	}

	DrawLine(-26, 0, 26, 0, GREEN);

    EndMode2D();
}