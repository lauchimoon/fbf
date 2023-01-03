#pragma once

#include <map>
#include <string>
#include "button.h"
#include "textbox.h"
#include "state.h"

struct UI {
    Rectangle outline_clip;
    Rectangle outline_components;
    Rectangle outline_color;
    Textbox box_title;
    Textbox box_fps;
    Textbox box_frame_dur;
    std::map<std::string, Button> buttons;
    Font font;
    Color brush_color;

    void draw(State state);
    void update(State *state);
    void end(void);
    void display_message(std::string msg);
    void reset(void);
};

UI ui_new(void);
