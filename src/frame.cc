#include "frame.h"
#include "config.h"

Frame frame_new(int id, std::string img_path)
{
    RenderTexture draw_rt = LoadRenderTexture(CLIP_SIZE_W, CLIP_SIZE_H);
    BeginTextureMode(draw_rt);
    ClearBackground(BLANK);
    EndTextureMode();
    return Frame{ id, img_path, draw_rt };
}

void Frame::write_components(void)
{}

void Frame::load_components(void)
{}

void Frame::end(void)
{
    UnloadRenderTexture(draw_texture);
    UnloadTexture(visible_texture);

    for (int i = 0; i < components.size(); i++) {
        components[i].end();
    }
}
