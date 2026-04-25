#include "window_midi_player.hh"

Camera2D _cam = { 0 };

float playing_time = 0.0f;

int notes_count = 0;
midi_player_note* notes = nullptr;

Texture2D background_texture_checkerboard;
Texture2D background_gradient;

float background_texture_x = 0.0f;
float background_texture_y = 0.0f;

void initialize_midi_player()
{
	piano_texture = LoadTexture("../assets/ui/UI_MidiPlayer_Piano.png");
	background_texture_checkerboard = LoadTexture("../assets/ui/UI_MidiPlayer_Background_Checkerboard.png");
	background_gradient = LoadTexture("../assets/ui/UI_MidiPlayer_Background_Gradient.png");

	load_note_sounds();
}

void unload_midi_player()
{
	UnloadTexture(piano_texture);
	UnloadTexture(background_texture_checkerboard);
	UnloadTexture(background_gradient);

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
		bool is_flat = flat_notes.find(sheet.pitch[i]) != flat_notes.end();
		float width = (is_flat) ? 0.5f : 1.0f;
		float height = (float)sheet.durations[i] * VERTICAL_SCALE;
		float x = pitch_to_position.at(sheet.pitch[i]) + ((is_flat) ? 0.25f : 0.0f);
		float y = -(float)sheet.timestamps_start[i] * VERTICAL_SCALE - height - INITIAL_DELAY * VERTICAL_SCALE;

		notes[i].position = { x, y };
		notes[i].size = { width, height };
		notes[i].pitch = sheet.pitch[i];
		notes[i].played = false;
		notes[i].is_flat = is_flat;

		printf("Note %d: position=(%f, %f), size=(%f, %f)\n", i, x, y, width, height);
	}
}

void update_midi_playback(const float deltaTime)
{
	playing_time += deltaTime;

	float texW = BACKGROUND_TEXTURE_SIZE * BACKGROUND_SCALE;
	float texH = BACKGROUND_TEXTURE_SIZE * BACKGROUND_SCALE;

	background_texture_x -= deltaTime * BACKGROUND_SPEED * cos(BACKGROUND_ROTATION * DEG2RAD);
	background_texture_y += deltaTime * BACKGROUND_SPEED * sin(BACKGROUND_ROTATION * DEG2RAD);

	background_texture_x = fmod(background_texture_x, texW);
	if (background_texture_x < 0) background_texture_x += texW;

	background_texture_y = fmod(background_texture_y, texH);
	if (background_texture_y < 0) background_texture_y += texH;

	for (int i = 0; i < notes_count; ++i)
	{
		notes[i].position.y += VERTICAL_SCALE * deltaTime;
		if (notes[i].position.y >= -notes[i].size.y && !notes[i].played)
		{
			notes[i].played = true;
			play_note_sound((unsigned int)notes[i].pitch, 2);
		}

		if (notes[i].position.y >= notes[i].size.y)
		{
			stop_note_sound((unsigned int)notes[i].pitch);
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

void draw_background()
{
	float texW = background_texture_checkerboard.width * BACKGROUND_SCALE;
	float texH = background_texture_checkerboard.height * BACKGROUND_SCALE;

	float angle = BACKGROUND_ROTATION * DEG2RAD;
	float cosA = cos(angle);
	float sinA = sin(angle);

	Vector2 screenCenter = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

	for (int x = -6; x <= 6; x++)
	{
		for (int y = -6; y <= 6; y++)
		{
			float localX = x * texW - background_texture_x;
			float localY = y * texH - background_texture_y;

			float rotX = localX * cosA - localY * sinA;
			float rotY = localX * sinA + localY * cosA;

			Vector2 pos = { screenCenter.x + rotX, screenCenter.y + rotY };

			Rectangle dest = {
				pos.x,
				pos.y,
				texW,
				texH
			};

			Vector2 origin = { texW / 2.0f, texH / 2.0f };

			DrawTexturePro(
				background_texture_checkerboard,
				{ 0, 0, (float)background_texture_checkerboard.width, (float)background_texture_checkerboard.height },
				dest,
				origin,
				BACKGROUND_ROTATION,
				COLOR_BACKGROUND
			);
		}
	}
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

void draw_midi_player_screen()
{
    ClearBackground(COLOR_BACKGROUND);

	draw_background();
	draw_background_gradient();

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
        DrawRectangleV(notes[i].position, notes[i].size, (notes[i].is_flat) ? COLOR_NOTE_FLAT : COLOR_NOTE);
	}

	DrawLine(-26, 0, 26, 0, BLACK);

    EndMode2D();
}