#pragma once
#include "sheet.hh"

#define COLOR_BACKGROUND (Color) { 88, 8, 94, 255 }
#define COLOR_BACKGROUND_DARK (Color) { 88, 8, 94, 255 }

void drawGrid(int xoffset, int yoffset, int col_width, int row_width);

void drawSoundTimeline(Sheet &sheet);

void initEditor();
