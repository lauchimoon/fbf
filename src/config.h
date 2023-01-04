#pragma once

enum {
    WIDTH = 1152,
    HEIGHT = 860,
    TEXTSIZE = 128,
    CLIP_SIZE_W = 740,
    CLIP_SIZE_H = 580,
    COMPONENTS_BOXSIZE_W = 190,
    COMPONENTS_BOXSIZE_H = CLIP_SIZE_H,
    MIN_FPS_VALUE = 1,
    MAX_FPS_VALUE = 1000,
    MESSAGE_DURATION = 120
};
#define TITLE "fbf"
#define DEFAULT_FRAME_NEW(id) frame_new(id, "(none)")

