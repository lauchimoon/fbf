#pragma once

#include "raylib.h"

struct Textbox {
    Rectangle bounds;
    char *text;
    bool edit;

    bool draw(bool numerical);
    void end(void);
};

Textbox textbox_new(Rectangle bounds);
