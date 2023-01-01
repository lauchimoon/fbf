#pragma once

struct State {
    int current_frame;
    int nframes;
    int fps_value;
    const char *animtitle;
};

State state_new();

