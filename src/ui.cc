#include "config.h"
#include "ui.h"
#include "raygui.h"

UI ui_new(void)
{
    UI ui = { 0 };
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

    ui.outline_clip = clip_outline;
    ui.outline_components = components_outline;
    ui.outline_color = color_outline;
    ui.box_title = box_title;
    ui.box_fps = box_fps;
    ui.buttons = buttons;
    return ui;
}

void UI::draw(State state)
{
    // Title box
    box_title.draw();

    if (std::string(box_title.text).empty()) // Placeholder text
        DrawTextEx(font, "Title", Vector2{box_title.bounds.x + 5.0f, box_title.bounds.y + 8.0f}, 32.0f, 2.0f, Fade(GRAY, 0.5f));

    // Buttons
    for (auto b : buttons)
        b.second.draw();

    // Clip outline
    DrawRectangleLinesEx(outline_clip, 2, BLACK);

    // Components outline
    DrawRectangleLinesEx(outline_components, 2, BLACK);

    // Color picking
    brush_color = GuiColorPicker(outline_color, NULL, brush_color);
    DrawRectangleLinesEx(outline_color, 2, BLACK);

    // Frame number
    const char *frame_n_text = TextFormat("Frame %d", state.current_frame + 1);
    Vector2 ntext_measures = MeasureTextEx(font, frame_n_text, 64.0f, 2.0f);
    DrawTextEx(font, frame_n_text, Vector2{float(GetScreenWidth()/2 - ntext_measures.x/2.0f), outline_clip.y + outline_clip.height + 10.0f}, 64.0f, 2.0f, BLACK);

    // Change FPS
    box_fps.draw(&state.fps_value);
    Vector2 fpstext_measures = MeasureTextEx(font, "FPS", 64.0f, 2.0f);
    DrawTextEx(font, "FPS", Vector2{(outline_components.x + outline_components.width)/2.0f - fpstext_measures.x/2.0f, box_fps.bounds.y - 70.0f}, 64.0f, 2.0f, BLACK);
}

void UI::update(State *state)
{
    box_title.edit = CheckCollisionPointRec(GetMousePosition(), box_title.bounds);
    box_fps.edit = CheckCollisionPointRec(GetMousePosition(), box_fps.bounds);

    // Change current frame number
    if (buttons["<<"].pressed()) {
        state->current_frame = 0;
    } else if (buttons["<"].pressed()) {
        state->current_frame--;
    } else if (buttons[">"].pressed()) {
        if (state->current_frame == state->nframes - 1)
            state->nframes++;

        state->current_frame++;
    } else if (buttons[">>"].pressed()) {
        state->current_frame = state->nframes - 1;
    }
}
