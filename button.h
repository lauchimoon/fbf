#pragma once

#include "raylib.h"

struct Button {
    Rectangle bounds;
    const char *text;

    void draw();
    bool pressed();
};

Button button_new(Rectangle bounds, const char *text);
