#pragma once

#include <string>
#include "raylib.h"

struct Frame {
    int id;
    std::string img_path;
    //RenderTexture img_texture;
    RenderTexture draw_texture;

    void end(void);
};

Frame frame_new(int id, std::string img_path);
