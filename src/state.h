#pragma once

#include <string>

struct State {
    int current_frame;
    int nframes;
    int fps_value;
    std::string animtitle;
};

State state_new();

