#pragma once
#include "sheet.hh"
#include "window_common.hh"
#include "note_player.hh"

void drawGrid(int xoffset, int yoffset, int col_width, int row_width);

void drawSoundTimeline(Sheet &sheet);

void initEditor();
