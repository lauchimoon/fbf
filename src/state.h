#pragma once

#include "frame.h"
#include "components.h"
#include <vector>
#include <string>

struct State {
    int current_frame;
    int nframes;
    int anim_fps;
    std::string anim_title;
    std::vector<Frame> frames;
    bool saved;
    int component_selected;
    std::string project_dirname;
    ComponentManager comp_manager;

    int read(std::vector<std::string> info, std::vector<std::string> info_comp);
    void write(void);
    void end(void);
};

State state_new();

