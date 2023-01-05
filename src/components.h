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
    bool resizable;
    bool selected;
    int font_size;
    Color color;
    Texture texture;
};

