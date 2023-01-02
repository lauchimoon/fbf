#pragma once

#include "frame.h"
#include <vector>
#include <string>

struct State {
    int current_frame;
    int nframes;
    int anim_fps;
    std::string anim_title;
    std::vector<Frame> frames;
    bool saved;

    int read(std::vector<std::string> info);
    void write(void);
};

State state_new();

