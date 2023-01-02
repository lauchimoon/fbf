#include "frame.h"

Frame frame_new(int id, std::string img_path, int duration)
{
    return Frame{ id, img_path, duration };
}
