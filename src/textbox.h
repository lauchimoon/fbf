#pragma once

#include "raylib.h"

struct Textbox {
    Rectangle bounds;
    char *text;
    bool edit;

    bool draw();
    bool draw(int *value);
};

Textbox textbox_new(Rectangle bounds, char *text);
