#include <stdio.h>
#include <string>
#include <map>
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

enum {
    WIDTH = 1152,
    HEIGHT = 860,
    TEXTSIZE = 128,
    CLIP_SIZE_W = 740,
    CLIP_SIZE_H = 580,
    COMPONENTS_BOXSIZE_W = 190,
    COMPONENTS_BOXSIZE_H = CLIP_SIZE_H,
    MIN_FPS_VALUE = 1,
    MAX_FPS_VALUE = 1000
};
#define TITLE "fbf"

struct State {
    int current_frame;
    int nframes;
    int fps_value;
    const char *animtitle;
};

struct Button {
    Rectangle bounds;
    const char *text;

    void draw() { GuiButton(bounds, text); }

    bool pressed()
    {
        if (CheckCollisionPointRec(GetMousePosition(), bounds))
            return IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        return false;   
    }
};

struct Textbox {
    Rectangle bounds;
    char *text;
    bool edit;

    bool draw() { return GuiTextBox(bounds, text, TEXTSIZE, edit); }

    bool draw(int *value) { return GuiValueBox(bounds, text, value, MIN_FPS_VALUE, MAX_FPS_VALUE, edit); }
};

State state_new()
{
    return State{ 0, 1, 20, "" };
}

inline Button button_new(Rectangle bounds, const char *text)
{
    return Button{ bounds, text };
}

inline Textbox textbox_new(Rectangle bounds, char *text)
{
    return Textbox{ bounds, text, false };
}

int main()
{
    SetTraceLogLevel(LOG_NONE);
    InitWindow(WIDTH, HEIGHT, TITLE);

    Font font = LoadFont("assets/JandaManateeSolid.ttf");
    GuiSetFont(font);

    Rectangle clip_outline = { float(GetScreenWidth()/2 - CLIP_SIZE_W/2), float(GetScreenHeight()/2 - CLIP_SIZE_H/2), float(CLIP_SIZE_W), float(CLIP_SIZE_H) };
    Rectangle components_outline = { float(clip_outline.x - (COMPONENTS_BOXSIZE_W + 10)), float(clip_outline.y), COMPONENTS_BOXSIZE_W, COMPONENTS_BOXSIZE_H - 140.0f };
    Rectangle color_outline = { float(clip_outline.x + clip_outline.width + 10), float(clip_outline.y), COMPONENTS_BOXSIZE_W - 30.0f, COMPONENTS_BOXSIZE_H };

    char box_title_text[TEXTSIZE] = { 0 };
    char box_fps_text[TEXTSIZE] = { 0 };
    Textbox box_title = textbox_new({10.0f, 10.0f, 465.0f, 48.0f}, box_title_text);
    Textbox box_fps = textbox_new({components_outline.x + 5.0f, components_outline.y + 387.0f, components_outline.width - 10.0f, 48.0f}, box_fps_text);

    std::map<std::string, Button> buttons = {
        {"new", button_new({10.0f, 70.0f, 148.0f, 48.0f}, "New Project")},
        {"open", button_new({168.0f, 70.0f, 148.0f, 48.0f}, "Open Project")},
        {"save", button_new({326.0f, 70.0f, 148.0f, 48.0f}, "Save Project")},
        {"egif", button_new({box_title.bounds.x + box_title.bounds.width + 10.0f, box_title.bounds.y, 148.0f, 48.0f}, "Export GIF")},
        {"emp4", button_new({box_title.bounds.x + box_title.bounds.width + 168.0f, box_title.bounds.y, 148.0f, 48.0f}, "Export MP4")},
        {"imag", button_new({components_outline.x + 5.0f, components_outline.y + 5.0f, components_outline.width - 10.0f, 64.0f}, "Image")},
        {"draw", button_new({components_outline.x + 5.0f, components_outline.y + 80.0f, components_outline.width - 10.0f, 64.0f}, "Draw")},
        {"erase", button_new({components_outline.x + 5.0f, components_outline.y + 155.0f, components_outline.width - 10.0f, 64.0f}, "Erase")},
        {"text", button_new({components_outline.x + 5.0f, components_outline.y + 230.0f, components_outline.width - 10.0f, 64.0f}, "Text")},
        {"<<", button_new({clip_outline.x + 210.0f, clip_outline.y + clip_outline.height + 70.0f, 64.0f, 64.0f}, "<<")},
        {"<", button_new({clip_outline.x + 290.0f, clip_outline.y + clip_outline.height + 70.0f, 64.0f, 64.0f}, "<")},
        {">", button_new({clip_outline.x + 370.0f, clip_outline.y + clip_outline.height + 70.0f, 64.0f, 64.0f}, ">")},
        {">>", button_new({clip_outline.x + 450.0f, clip_outline.y + clip_outline.height + 70.0f, 64.0f, 64.0f}, ">>")},
        {"copy", button_new({25.0f, float(GetScreenHeight() - 242), 148.0f, 64.0f}, "Copy Frame")},
        {"paste", button_new({25.0f, float(GetScreenHeight() - 169), 148.0f, 64.0f}, "Paste Frame")},
        {"del", button_new({25.0f, float(GetScreenHeight() - 95), 148.0f, 64.0f}, "Delete Frame")},
    };

    State state = state_new();
    Color brush_color = BLACK;
    Texture fbf_logo = LoadTexture("assets/logo.png");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Updating
        // --------
        state.animtitle = box_title.text;
        box_title.edit = CheckCollisionPointRec(GetMousePosition(), box_title.bounds);
        box_fps.edit = CheckCollisionPointRec(GetMousePosition(), box_fps.bounds);

        if (state.current_frame <= 0) // Clamp
            state.current_frame = 0;
        if (state.nframes <= 1)
            state.nframes = 1;

        // Change current frame number
        if (buttons["<<"].pressed()) {
            state.current_frame = 0;
        } else if (buttons["<"].pressed()) {
            state.current_frame--;
        } else if (buttons[">"].pressed()) {
            if (state.current_frame == state.nframes - 1)
                state.nframes++;

            state.current_frame++;
        } else if (buttons[">>"].pressed()) {
            state.current_frame = state.nframes - 1;
        }

        // Drawing
        // -------
        BeginDrawing();

        ClearBackground(WHITE);

        // Title box
        box_title.draw();

        if (std::string(box_title.text).empty()) // Placeholder text
            DrawTextEx(font, "Title", Vector2{box_title.bounds.x + 5.0f, box_title.bounds.y + 8.0f}, 32.0f, 2.0f, Fade(GRAY, 0.5f));

        // Buttons
        for (auto b : buttons)
            b.second.draw();

        // Clip outline
        DrawRectangleLinesEx(clip_outline, 2, BLACK);

        // Components outline
        DrawRectangleLinesEx(components_outline, 2, BLACK);

        // Color picking
        brush_color = GuiColorPicker(color_outline, NULL, brush_color);
        DrawRectangleLinesEx(color_outline, 2, BLACK);

        // Frame number
        const char *frame_n_text = TextFormat("Frame %d", state.current_frame + 1);
        Vector2 ntext_measures = MeasureTextEx(font, frame_n_text, 64.0f, 2.0f);
        DrawTextEx(font, frame_n_text, Vector2{float(GetScreenWidth()/2 - ntext_measures.x/2.0f), clip_outline.y + clip_outline.height + 10.0f}, 64.0f, 2.0f, BLACK);

        // Change FPS
        box_fps.draw(&state.fps_value);
        Vector2 fpstext_measures = MeasureTextEx(font, "FPS", 64.0f, 2.0f);
        DrawTextEx(font, "FPS", Vector2{(components_outline.x + components_outline.width)/2.0f - fpstext_measures.x/2.0f, box_fps.bounds.y - 70.0f}, 64.0f, 2.0f, BLACK);

        // Draw logo
        DrawTexture(fbf_logo, GetScreenWidth() - 200, 15, WHITE);

        EndDrawing();
    }

    UnloadFont(font);
    UnloadTexture(fbf_logo);
    CloseWindow();
    return 0;
}

