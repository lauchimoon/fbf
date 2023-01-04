#pragma once

#include "raylib.h"

enum {
    COMP_TYPE_IMG = 0,
    COMP_TYPE_DRAW,
    COMP_TYPE_ERASE,
    COMP_TYPE_TEXT
};

struct Component {
    int id;
    int type;
    bool movable;
    bool resizable;
    int thickness;
    int font_size;
    bool is_clicked;
    Color color;
    Texture texture;
};

