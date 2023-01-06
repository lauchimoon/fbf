#pragma once

#include <string>
#include <vector>
#include "raylib.h"
#include "components.h"

struct Frame {
    int id;
    std::string img_path;
    //RenderTexture img_texture;
    RenderTexture draw_texture;
    Texture visible_texture;
    std::vector<Component> components;

    void write_components(void);
    void load_components(void);
    void end(void);
};

Frame frame_new(int id, std::string img_path);
