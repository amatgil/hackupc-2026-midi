#pragma once
#include "sheet.hh"
#include "window_common.hh"
#include "note_player.hh"

typedef struct {
	float time;
	int pitch;
	bool played;
} editorPlaybackNote;

#define COLOR_GUI (Color){212, 16, 230, 255}
#define COLOR_GUI_DARK (Color){131, 13, 151, 255}
#define COLOR_BUTTON (Color){227, 18, 243, 255}

void drawGrid(int xoffset, int yoffset, int col_width, int row_width);

void updateMidiEditor(const float deltaTime);

void drawSoundTimeline(Sheet &sheet);

void initEditor();
