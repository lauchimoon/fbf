#include "config.h"
#include "ui.h"
#include "raygui.h"
#include "tinyfiledialogs.h"
#include <string.h>
#include <math.h>
#include <sstream>

int show_msg = 0;
int msg_time = MESSAGE_DURATION;
std::string msg = "";
char stateload_filename[TEXTSIZE] = { 0 };

enum {
    msg_tmp = 1,
    msg_forever,
    msg_decision
};

int ndigits(int n);
int file_dialog(const char *title, char *filename, const char *filters, const char *message);
std::string read_file_to_str(const char *filename);
std::vector<std::string> split_str(std::string s, char find);

UI ui_new(void)
{
    UI ui = { 0 };
    Rectangle clip_outline = { float(GetScreenWidth()/2 - CLIP_SIZE_W/2), float(GetScreenHeight()/2 - CLIP_SIZE_H/2), float(CLIP_SIZE_W), float(CLIP_SIZE_H) };
    Rectangle components_outline = { float(clip_outline.x - (COMPONENTS_BOXSIZE_W + 10)), float(clip_outline.y), COMPONENTS_BOXSIZE_W, COMPONENTS_BOXSIZE_H - 140.0f };
    Rectangle color_outline = { float(clip_outline.x + clip_outline.width + 10), float(clip_outline.y), COMPONENTS_BOXSIZE_W - 30.0f, COMPONENTS_BOXSIZE_H - 40.0f };
    Rectangle alpha_outline = { float(clip_outline.x + clip_outline.width + 10), float(clip_outline.y + clip_outline.height) - 35.0f, COMPONENTS_BOXSIZE_W - 5.0f, 30.0f };

    Textbox box_title = textbox_new({10.0f, 10.0f, 465.0f, 48.0f});
    Textbox box_fps = textbox_new({components_outline.x + 5.0f, components_outline.y + 387.0f, components_outline.width - 10.0f, 48.0f});

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
        {"prev", button_new({clip_outline.x, clip_outline.y + clip_outline.height, 148.0f, 48.0f}, "Preview")},
    };

    Textbox box_frame_dur = textbox_new({buttons[">>"].bounds.x + 70.0f, buttons[">>"].bounds.y + 8.0f, components_outline.width + 28.0f, 48.0f});

    ui.outline_clip = clip_outline;
    ui.outline_components = components_outline;
    ui.outline_color = color_outline;
    ui.outline_alpha = alpha_outline;
    ui.box_title = box_title;
    ui.box_fps = box_fps;
    ui.buttons = buttons;

    ui.brush_color = BLACK;
    ui.alpha_value = 1.0f;

    ui.box_fps.text[0] = '2';
    ui.box_fps.text[1] = '0';

    show_msg = 0;
    msg_time = MESSAGE_DURATION;
    msg = "";

    return ui;
}

void UI::draw(State state)
{
    // Title box
    box_title.draw(false);

    if (std::string(box_title.text).empty()) // Placeholder text
        DrawTextEx(font, "Title", Vector2{box_title.bounds.x + 5.0f, box_title.bounds.y + 8.0f}, 32.0f, 2.0f, Fade(GRAY, 0.5f));

    // Buttons
    for (auto b : buttons) {
        if (b.first != "prev") { // Preview button has custom colors
            b.second.draw();
        }
    }

    // Change preview button style
    int border_color_normal_prev = GuiGetStyle(BUTTON, BORDER_COLOR_NORMAL);
    int base_color_normal_prev = GuiGetStyle(BUTTON, BASE_COLOR_NORMAL);
    int text_color_normal_prev = GuiGetStyle(BUTTON, TEXT_COLOR_NORMAL);

    int border_color_focused_prev = GuiGetStyle(BUTTON, BORDER_COLOR_FOCUSED);
    int base_color_focused_prev = GuiGetStyle(BUTTON, BASE_COLOR_FOCUSED);
    int text_color_focused_prev = GuiGetStyle(BUTTON, TEXT_COLOR_FOCUSED);

    int border_color_pressed_prev = GuiGetStyle(BUTTON, BORDER_COLOR_PRESSED);
    int base_color_pressed_prev = GuiGetStyle(BUTTON, BASE_COLOR_PRESSED);
    int text_color_pressed_prev = GuiGetStyle(BUTTON, TEXT_COLOR_PRESSED);

    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt(DARKGREEN));
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(LIME));
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(Color{ LIGHTGRAY.r + 25, LIGHTGRAY.g + 25, LIGHTGRAY.b + 25, 255 }));
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, ColorToInt(LIME));
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(GREEN));
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
    GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, ColorToInt(GREEN));
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(Color{ 157, 255, 150, 255 }));
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(WHITE));

    buttons["prev"].draw();

    // Return to normal
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, border_color_normal_prev);
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, base_color_normal_prev);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, text_color_normal_prev);
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, border_color_focused_prev);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, base_color_focused_prev);
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, text_color_focused_prev);
    GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, border_color_pressed_prev);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, base_color_pressed_prev);
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, text_color_pressed_prev);

    // Clip outline
    DrawRectangleLinesEx(outline_clip, 2, BLACK);

    // Components outline
    DrawRectangleLinesEx(outline_components, 2, BLACK);

    // Color and alpha picking
    brush_color = GuiColorPicker(outline_color, NULL, brush_color);
    DrawRectangleLinesEx(outline_color, 2, BLACK);
    alpha_value = GuiColorBarAlpha(outline_alpha, NULL, alpha_value);

    // Frame number
    const char *frame_n_text = TextFormat("Frame %d", state.current_frame + 1);
    Vector2 ntext_measures = MeasureTextEx(font, frame_n_text, 64.0f, 2.0f);
    DrawTextEx(font, frame_n_text, Vector2{float(GetScreenWidth()/2 - ntext_measures.x/2.0f), outline_clip.y + outline_clip.height + 10.0f}, 64.0f, 2.0f, BLACK);

    // Change FPS
    box_fps.draw(true);
    Vector2 fpstext_measures = MeasureTextEx(font, "FPS", 64.0f, 2.0f);
    DrawTextEx(font, "FPS", Vector2{(outline_components.x + outline_components.width)/2.0f - fpstext_measures.x/2.0f, box_fps.bounds.y - 70.0f}, 64.0f, 2.0f, BLACK);

    // Warn that project is not saved
    if (!state.saved) {
        DrawTextEx(font, "*", Vector2{buttons["save"].bounds.x + buttons["save"].bounds.width + 10.0f, buttons["save"].bounds.y}, 64.0f, 2.0f, DARKGRAY);
    }

    // Display messages
    if (show_msg == msg_tmp) {
        msg_time--;
        display_message(msg);

        if (msg_time <= 0) {
            msg_time = MESSAGE_DURATION;
            show_msg = 0;
        }
    } else if ((show_msg == msg_forever) || (show_msg == msg_decision)) {
        msg_time = 1;
        display_message(msg);
    } else {
        show_msg = 0;
        msg_time = MESSAGE_DURATION;
    }
}

void UI::update(State *state)
{
    box_title.edit = CheckCollisionPointRec(GetMousePosition(), box_title.bounds);
    box_fps.edit = CheckCollisionPointRec(GetMousePosition(), box_fps.bounds);

    state->nframes = state->frames.size();

    state->frames[state->current_frame].id = state->current_frame;

    // Change current frame number
    if (buttons["<<"].pressed()) {
        state->current_frame = 0;
        state->saved = false;
    } else if (buttons["<"].pressed()) {
        state->current_frame--;
        state->saved = false;
    } else if (buttons[">"].pressed()) {
        if (state->current_frame == state->nframes - 1) {
            state->frames.push_back(DEFAULT_FRAME_NEW(state->current_frame + 1));
        }

        state->current_frame++;
        state->saved = false;
    } else if (buttons[">>"].pressed()) {
        state->current_frame = state->nframes - 1;
        state->saved = false;
    }

    // New project
    if (buttons["new"].pressed()) {
        if (state->saved) {
            (*state) = state_new();
            reset();
        } else {
            show_msg = msg_decision;
            msg = "Are you sure? Press Y/N";
        }
    }

    if (show_msg == msg_decision) {
        if (IsKeyPressed(KEY_Y)) { // Reset
            (*state) = state_new();
            reset();
        } else if (IsKeyPressed(KEY_N)) { // Just continue
            show_msg = 0;
            msg = "";
        }
    }

    // Load project
    if (buttons["open"].pressed()) {
        int result = file_dialog("Open fbf project", stateload_filename, "*.fbfp", "fbf project (*.fbfp)");
        if (result == 0) {
            std::string fileread = read_file_to_str(stateload_filename);
            std::vector<std::string> info = split_str(fileread, '\n');
            state->read(info);

            for (int i = 0; i < state->anim_title.length(); i++) {
                box_title.text[i] = state->anim_title[i];
            }

            for (int i = 0; i < ndigits(state->anim_fps); i++) {
                box_fps.text[i] = std::to_string(state->anim_fps)[i];
            }
        } else {
            show_msg = msg_tmp;
            msg = "Nothing was loaded";
        }
    }

    // Save project
    if (buttons["save"].pressed()) {
        if (!std::string(box_title.text).empty() && state->anim_fps > 0) {
            state->saved = true;
            state->anim_title = box_title.text;
            state->anim_fps = std::atoi(box_fps.text);
            state->write();
            show_msg = msg_tmp;
            msg = "Saved project";
        } else {
            show_msg = msg_tmp;
            msg = "Make sure to set all fields with a value";
        }
    }
}

void UI::end(void)
{
    box_title.end();
    box_fps.end();
}

void UI::display_message(std::string msg)
{
    if (msg_time >= 0) {
        DrawTextEx(font, msg.c_str(), Vector2{buttons["save"].bounds.x + buttons["save"].bounds.width + 60.0f, buttons["save"].bounds.y + 15.0f}, 32.0f, 1.0f, BLACK);
    }
}

void UI::reset(void)
{
    box_title.end();
    box_fps.end();
    box_title = textbox_new({10.0f, 10.0f, 465.0f, 48.0f});
    box_fps = textbox_new({outline_components.x + 5.0f, outline_components.y + 387.0f, outline_components.width - 10.0f, 48.0f});

    box_fps.text[0] = '2';
    box_fps.text[1] = '0';

    show_msg = 0;
    msg_time = MESSAGE_DURATION;
    msg = "";
}

int ndigits(int n)
{
    return int(log10(n) + 1);
}

int file_dialog(const char *title, char *filename, const char *filters, const char *message)
{
    int result = -1;
    int filter_count = 0;
    const char **filter_split = TextSplit(filters, ';', &filter_count);
    const char *tmp_filename = tinyfd_openFileDialog(title, filename, filter_count, filter_split, message, 0);

    if (tmp_filename != NULL)
        strcpy(filename, tmp_filename);
    else
        return 1;

    return 0;
}

std::string read_file_to_str(const char *filename)
{
    char line[1024] = { 0 };
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("error\n");
        return "";
    }
    std::string read = "";

    while (fgets(line, 1024, f)) {
        line[strcspn(line, "\n")] = 0;
        read += line;
        read += "\n";
    }

    read.pop_back();
    fclose(f);
    return read;
}

std::vector<std::string> split_str(std::string s, char find)
{
    std::vector<std::string> split = {};
    std::stringstream ss(s);
    std::string token;

    while (std::getline(ss, token, find)) {
        split.push_back(token);
    }

    return split;
}

