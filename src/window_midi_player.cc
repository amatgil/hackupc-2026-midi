#include "window_midi_player.hh"

Camera2D _cam = { 0 };

float playing_time = 0.0f;

int notes_count = 0;
midi_player_note* notes = nullptr;

Texture2D background_texture_checkerboard;
Texture2D piano_white_keys;
Texture2D piano_black_keys;

float background_texture_x = 0.0f;
float background_texture_y = 0.0f;

bool played_keys[88] = { false };

bool pedal_active = false;

void initialize_midi_player()
{
	piano_texture = LoadTexture("../assets/ui/UI_MidiPlayer_Piano.png");
	background_texture_checkerboard = LoadTexture("../assets/ui/UI_MidiPlayer_Background_Checkerboard.png");
	piano_white_keys = LoadTexture("../assets/ui/UI_MidiPlayer_Piano_WhiteKeys.png");
	piano_black_keys = LoadTexture("../assets/ui/UI_MidiPlayer_Piano_BlackKeys.png");

	load_note_sounds();
}

void unload_midi_player()
{
	UnloadTexture(piano_texture);
	UnloadTexture(background_texture_checkerboard);
	UnloadTexture(piano_white_keys);
	UnloadTexture(piano_black_keys);

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
		int pitch_adjusted = sheet.pitch[i];
		if (pitch_adjusted > 87) pitch_adjusted = 87;
		if (pitch_adjusted < 0) pitch_adjusted = 0;

		bool is_flat = flat_notes.find(pitch_adjusted) != flat_notes.end();
		float width = (is_flat) ? 0.5f : 1.0f;
		float height = (float)sheet.durations[i] * VERTICAL_SCALE;

		printf("Pitch: %d\n", pitch_adjusted);

		float x = pitch_to_position.at(pitch_adjusted) + ((is_flat) ? 0.25f : 0.0f);
		float y = -(float)sheet.timestamps_start[i] * VERTICAL_SCALE - height - INITIAL_DELAY * VERTICAL_SCALE;

		notes[i].position = { x, y };
		notes[i].size = { width, height };
		notes[i].pitch = pitch_adjusted;
		notes[i].played = false;
		notes[i].finished = false;
		notes[i].is_flat = is_flat;

		printf("Note %d: position=(%f, %f), size=(%f, %f)\n", i, x, y, width, height);
	}
}

void update_midi_playback(const float deltaTime)
{
	playing_time += deltaTime;

	if (IsKeyPressed(KEY_T)) pedal_active = !pedal_active;

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
			played_keys[notes[i].pitch] = true;
			play_note_sound((unsigned int)notes[i].pitch, 2);
		}

		if (notes[i].position.y >= notes[i].size.y && !notes[i].finished)
		{
			notes[i].finished = true;
			played_keys[notes[i].pitch] = false;
			if (!pedal_active) stop_note_sound((unsigned int)notes[i].pitch);
		}
	}
}

void draw_piano()
{
	float screen_w = (float)GetScreenWidth();
	float screen_h = (float)GetScreenHeight();
	float piano_y = screen_h * (1.0f - PIANO_VERTICAL_WINDOW_PROPORTION);
	float piano_h = screen_h * PIANO_VERTICAL_WINDOW_PROPORTION;

	Vector2 origin = { 0, 0 };

	Rectangle dest = { 0, piano_y, screen_w, piano_h };
	DrawTexturePro(piano_texture, { 0, 0, 2844, 350 }, dest, origin, 0, WHITE);
}

void draw_pressed_keys()
{
	float screen_w = (float)GetScreenWidth();
	float screen_h = (float)GetScreenHeight();
	float piano_y = screen_h * (1.0f - PIANO_VERTICAL_WINDOW_PROPORTION);
	float piano_h = screen_h * PIANO_VERTICAL_WINDOW_PROPORTION;

	Vector2 origin = { 0, 0 };

	for (int i = 0; i < 88; ++i)
	{
		if (played_keys[i])
		{
			bool is_flat = flat_notes.find(i) != flat_notes.end();

			// 1. Ampliem una mica el retall de la tecla negra perquè agafi tota l'ombra/vores del PNG. 
			// Pots jugar amb aquest 0.6f fins que quedi clavada.
			float world_width = is_flat ? 0.6f : 1.0f;

			// 2. Recuperem l'offset perquè la X comenci just on toca
			float x_offset = is_flat ? 0.25f : 0.0f;

			// Sumem l'offset aquí
			float normalized_x = pitch_to_position.at(i) + 26.0f + x_offset;

			float tex_pixels_per_unit = 2844.0f / 52.0f;

			Rectangle source_key = {
				normalized_x * tex_pixels_per_unit,
				0,
				world_width * tex_pixels_per_unit,
				350
			};

			float screen_pixels_per_unit = screen_w / 52.0f;

			Rectangle dest_key = {
				normalized_x * screen_pixels_per_unit,
				piano_y,
				world_width * screen_pixels_per_unit,
				piano_h
			};

			DrawTexturePro(
				is_flat ? piano_black_keys : piano_white_keys,
				source_key, dest_key, origin, 0, is_flat ? COLOR_NOTE_FLAT : COLOR_NOTE
			);
		}
	}
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

void draw_midi_player_screen()
{
    ClearBackground(COLOR_BACKGROUND);

	draw_background();
	draw_background_gradient(WHITE);

    BeginMode2D(_cam);

	_cam.target = { 0, 0 };
	_cam.offset = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	_cam.rotation = 0.0f;
	_cam.zoom = (float)GetScreenWidth() / 52.0f;

	Vector2 worldCenterPos = GetScreenToWorld2D({ 0, (float)GetScreenHeight() * (1.0f - PIANO_VERTICAL_WINDOW_PROPORTION) }, _cam);
	_cam.target = { 0, -worldCenterPos.y };

    for (int i = 0; i < notes_count; ++i)
    {
		Vector2 inf = GetWorldToScreen2D({notes[i].position.x, notes[i].position.y + notes[i].size.y}, _cam);
        if (inf.y < 0 || notes[i].position.y > 0) continue;
		DrawRectangleV(notes[i].position, notes[i].size, (notes[i].is_flat) ? COLOR_NOTE_FLAT : COLOR_NOTE);
	}


	DrawLine(-26, 0, 26, 0, BLACK);

    EndMode2D();

	draw_piano();

	draw_pressed_keys();
}