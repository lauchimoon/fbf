#include "components.h"
#include <iostream>

int get_type(std::string s);
std::vector<std::string> split_str(std::string s, char find);

Component component_new(std::string name, int nframe, std::string img_path, Vector2 pos)
{
    Texture t = LoadTexture(img_path.c_str());
    return Component{ name, nframe, COMP_TYPE_IMG, false, img_path, "", Rectangle{ pos.x, pos.y, float(t.width), float(t.height) }, WHITE, t };
}

Component component_new(std::string name, int nframe, std::string text, Rectangle box, Color color)
{
    return Component{ name, nframe, COMP_TYPE_TEXT, false, "", text, box, color, Texture{} };
}

void Component::end(void)
{
    UnloadTexture(texture);
}

ComponentManager::ComponentManager()
{}

ComponentManager::~ComponentManager()
{}

void ComponentManager::read(std::vector<std::string> info)
{
    for (int i = 1; i < info.size(); i++) {
        std::vector<std::string> sp = split_str(info[i], ':');

        std::string name = sp[0];
        m[name].name = name;
        m[name].nframe = std::atoi(sp[1].c_str());
        m[name].type = get_type(sp[2]);
        m[name].selected = false;
        if (m[name].type == COMP_TYPE_IMG) {
            m[name].img_path = sp[3];
            m[name].text = "";
            m[name].box = Rectangle{ float(std::atoi(sp[4].c_str())), float(std::atoi(sp[5].c_str())), float(std::atoi(sp[6].c_str())), float(std::atoi(sp[7].c_str())) };
            m[name].color = WHITE;
            m[name].texture = Texture{};
        } else if (m[name].type == COMP_TYPE_TEXT) {
            m[name].img_path = "";
            m[name].text = sp[3];
            m[name].box = Rectangle{ float(std::atoi(sp[4].c_str())), float(std::atoi(sp[5].c_str())), float(std::atoi(sp[6].c_str())), float(std::atoi(sp[7].c_str())) };
            long color = std::strtoul(sp[8].c_str(), 0, 16);
            m[name].color = GetColor(color);
            m[name].texture = Texture{};
        }
    }
}

void ComponentManager::write(void)
{}

Component ComponentManager::get(std::string name)
{
    return m[name];
}

int get_type(std::string s)
{
    if (s == "img")
        return COMP_TYPE_IMG;
    else if (s == "txt")
        return COMP_TYPE_TEXT;
    
    return -1;
}
