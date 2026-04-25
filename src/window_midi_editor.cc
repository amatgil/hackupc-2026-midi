#include "window_midi_editor.hh"
#include "raygui.h"
#include "raymath.h"
#include "utils.hh"
#include <array>

enum Tools {
    Move = 0,
    Delete,
    Split,
    Create,
    Volume,
};

static int xscroll_offset = 0;
static int yscroll_offset = 0;
const int pixels_per_second = 100;
const int row_width = 20;

static int dragging_note = -1;
static Vector2 dragging_offset = {0, 0};
static Tools tool = Move;
static float new_note_duration = 1.0f;
Sheet palette_sheet;
static array<Texture, 6> cursors;

void drawGrid(int xoffset, int yoffset, int col_width, int row_width) {
    int h = GetRenderHeight();
    int w = GetRenderWidth();
    int xfrac_off = xoffset % col_width;
    int yfrac_off = yoffset % row_width;
    for (int i = 0; i < (w / col_width) + 1; i++) {
        DrawLine(i * col_width + xfrac_off, 0, i * col_width + xfrac_off, h,
                 BLACK);
    }
    for (int i = 0; i < (h / row_width) + 1; i++) {
        DrawLine(0, i * row_width + yfrac_off, w, i * row_width + yfrac_off,
                 BLACK);
    }
}

Rectangle getNoteRect(const Sheet &sheet, int i) {
    return Rectangle{sheet.timestamps_start[i] * pixels_per_second +
                         xscroll_offset,
                     (float)sheet.pitch[i] * row_width + yscroll_offset,
                     sheet.durations[i] * pixels_per_second, row_width - 1};
}

void moveTool(Vector2 mPos, Rectangle &nRec, Sheet &sheet, int i) {
    if (dragging_note == i) {
        DrawRectangleLines(nRec.x, nRec.y, nRec.width, nRec.height, ORANGE);
        DrawRectangleLines(nRec.x + 1, nRec.y + 1, nRec.width - 1,
                           nRec.height - 1, ORANGE);
        DrawRectangleLines(nRec.x + 2, nRec.y + 2, nRec.width - 2,
                           nRec.height - 2, ORANGE);
    }
    if (IsMouseButtonUp(MOUSE_LEFT_BUTTON))
        dragging_note = -1;

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) and dragging_note == -1 and
        CheckCollisionPointRec(mPos, nRec)) {
        dragging_note = i;
        dragging_offset.x = mPos.x - nRec.x;
        dragging_offset.y = mPos.y - nRec.y;
    }

    if (dragging_note == i) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            mPos.x -= xscroll_offset + dragging_offset.x;
            mPos.y -= yscroll_offset + dragging_offset.y;
            mPos.x /= pixels_per_second;
            mPos.y /= row_width;
            sheet.pitch[i] = (int)mPos.y;
            sheet.timestamps_start[i] = mPos.x;
        }
    }
}

void splitTool(Vector2 mPos, Rectangle nRec, Sheet &sheet, int i) {
    if (CheckCollisionPointRec(mPos, nRec) and
        IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        sheet.attack_velocities.push_back(sheet.attack_velocities[i]);
        float duration_cut = (mPos.x - nRec.x) / pixels_per_second;
        sheet.durations.push_back(sheet.durations[i] - duration_cut);
        sheet.durations[i] = duration_cut;
        sheet.pitch.push_back(sheet.pitch[i]);
        sheet.attack_velocities.push_back(sheet.attack_velocities[i]);
        sheet.timestamps_start.push_back(sheet.timestamps_start[i] +
                                         duration_cut);
    }
}

void removeNote(Sheet &sheet, int to_remove) {
    ut::swap(sheet.attack_velocities[to_remove],
             sheet.attack_velocities[sheet.attack_velocities.size() - 1]);
    sheet.attack_velocities.pop_back();
    ut::swap(sheet.pitch[to_remove], sheet.pitch[sheet.pitch.size() - 1]);
    sheet.pitch.pop_back();
    ut::swap(sheet.durations[to_remove],
             sheet.durations[sheet.durations.size() - 1]);
    sheet.durations.pop_back();
    ut::swap(sheet.timestamps_start[to_remove],
             sheet.timestamps_start[sheet.timestamps_start.size() - 1]);
    sheet.timestamps_start.pop_back();
}

void initEditor() {
    palette_sheet.attack_velocities = {10};
    palette_sheet.durations = {1.0f};
    palette_sheet.timestamps_start = {0.0f};
    palette_sheet.pitch = {0};

    cursors[0] = LoadTexture("assets/icons/boxicons--move.png");
    cursors[1] = LoadTexture("assets/icons/boxicons--eraser-filled.png");
    cursors[2] = LoadTexture("assets/icons/boxicons--cut-filled.png");
    cursors[3] = LoadTexture("assets/icons/boxicons--plus-circle-filled.png");
    cursors[4] = LoadTexture("assets/icons/boxicons--volume-full-filled.png");

    HideCursor();
}

void toolCreate(Sheet &sheet, Vector2 mPos) {
    palette_sheet.timestamps_start[0] =
        (mPos.x - xscroll_offset) / pixels_per_second;
    palette_sheet.pitch[0] = (mPos.y - yscroll_offset) / row_width;
    Rectangle nRec = getNoteRect(palette_sheet, 0);
    DrawRectangleRounded(nRec, 0.5, 5, Color{GRAY.r, GRAY.g, GRAY.b, 128});
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        sheet.attack_velocities.push_back(palette_sheet.attack_velocities[0]);
        sheet.pitch.push_back(palette_sheet.pitch[0]);
        sheet.durations.push_back(palette_sheet.durations[0]);
        sheet.timestamps_start.push_back(palette_sheet.timestamps_start[0]);
    }
}

void drawCursor() {
    Vector2 mPos = GetMousePosition();
    DrawTextureV(cursors[tool], mPos - Vector2{0, -16}, WHITE);
    float cross_size = 16;
    DrawLineEx(mPos + Vector2{0, cross_size}, mPos - Vector2{0, cross_size}, 3,
               BLACK);
    DrawLineEx(mPos + Vector2{cross_size, 0}, mPos - Vector2{cross_size, 0}, 3,
               BLACK);
}

void drawGUI() {
    float w = (float)GetScreenWidth();
    float h = (float)GetScreenHeight();

    DrawRectangle(0, 0, w, h * 0.1f, WHITE);
    DrawRectangle(0, h - h * 0.05f, w, h * 0.05f, WHITE);
}

void drawSoundTimeline(Sheet &sheet) {
    int h = GetRenderHeight();
    int w = GetRenderWidth();
    if (IsKeyDown(KEY_LEFT_CONTROL))
        yscroll_offset += GetMouseWheelMove() * 30;
    else
        xscroll_offset += GetMouseWheelMove() * 30;

    if (xscroll_offset > 0)
        xscroll_offset = 0;
    if (yscroll_offset < 0)
        yscroll_offset = 0;
    if (yscroll_offset > 87)
        yscroll_offset = 87;

    ClearBackground(COLOR_BACKGROUND);
    if (xscroll_offset > 0)
        BeginScissorMode(xscroll_offset, 0, w, h);
    ClearBackground(COLOR_BACKGROUND);
    EndScissorMode();

    drawGrid(xscroll_offset, yscroll_offset, pixels_per_second, row_width);

    Vector2 mPos = GetMousePosition();
    size_t size = sheet.timestamps_start.size();
    unsigned int to_remove = -1;
    if (tool == Create) {
        toolCreate(sheet, mPos);
    }

    for (int i = 0; i < size; i++) {
        Rectangle nRec = getNoteRect(sheet, i);
        DrawRectangleRounded(nRec, 0.5, 5, GRAY);
        BeginScissorMode(nRec.x,
                         nRec.y + row_width -
                             (sheet.attack_velocities[i] / 128.) * row_width,
                         nRec.width, nRec.height);
        DrawRectangleRounded(nRec, 0.5, 5, DARKGRAY);
        EndScissorMode();
        if (tool == Move)
            moveTool(mPos, nRec, sheet, i);
        else if (tool == Split) {
            splitTool(mPos, nRec, sheet, i);
        } else if (tool == Delete) {
            if (CheckCollisionPointRec(mPos, nRec) and
                IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                to_remove = i;
            }
        }

        if (tool == Volume) {
            if (CheckCollisionPointRec(mPos, nRec) and
                IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                dragging_note = i;
            }
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) and dragging_note == i) {
                sheet.attack_velocities[i] =
                    (1.0 - Clamp((mPos.y - nRec.y), 0., (float)row_width) /
                               row_width) *
                    128;
            }
            if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
                dragging_note = -1;
            }
        }
    }

    if (to_remove != -1) {
        removeNote(sheet, to_remove);
    }

    if (IsKeyReleased(KEY_S)) {
        tool = Split;
    } else if (IsKeyReleased(KEY_M)) {
        tool = Move;
    } else if (IsKeyPressed(KEY_E)) {
        tool = Delete;
    } else if (IsKeyPressed(KEY_C)) {
        tool = Create;
    } else if (IsKeyPressed(KEY_V)) {
        tool = Volume;
    }

    drawCursor();

    drawGUI();
}
