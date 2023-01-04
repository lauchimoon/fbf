#include "frame.h"

Frame frame_new(int id, std::string img_path)
{
    return Frame{ id, img_path };
}
