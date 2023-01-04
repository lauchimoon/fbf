#pragma once

#include <string>

struct Frame {
    int id;
    std::string img_path;
};

Frame frame_new(int id, std::string img_path);
