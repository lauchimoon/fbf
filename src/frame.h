#pragma once

#include <string>

struct Frame {
    int id;
    std::string img_path;
    int duration;
};

Frame frame_new(int id, std::string img_path, int duration);
