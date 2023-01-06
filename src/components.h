#pragma once

#include "raylib.h"
#include <string>
#include <vector>
#include <map>

enum {
    COMP_TYPE_IMG = 0,
    COMP_TYPE_DRAW,
    COMP_TYPE_ERASE,
    COMP_TYPE_TEXT
};

struct Component {
    std::string name;
    int nframe;
    int type;
    bool selected;
    std::string img_path;
    std::string text;
    Rectangle box;
    Color color;
    Texture texture;
};

class ComponentManager {
public:
    ComponentManager();
    ~ComponentManager();

    void read(std::vector<std::string> info);
    void write(void);
    Component get(std::string name);

    std::string project_at;

private:
    std::map<std::string, Component> m;
};
