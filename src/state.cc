#include "state.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <filesystem>
#include <ctype.h>

int find_char_begin(std::string s, char c);
std::string replace_all(std::string s, char c1, char c2);
std::string lowercase(std::string s);

State state_new()
{
    int current_frame = 0;
    int nframes = 1;
    int fps = 20;
    std::string title = "";
    std::vector<Frame> frames = {};
    bool saved = true;
    int component = -1;
    ComponentManager comp_manager = ComponentManager();

    frames.push_back(frame_new(0, "none"));

    return State{ current_frame, nframes, fps, title, frames, saved, component, "", comp_manager };
}

int State::read(std::vector<std::string> info, std::vector<std::string> info_comp)
{
    // Validate file
    if (info[0] != "fbf")
        return 1;

    // Load project info
    anim_title = info[1].substr(strlen("title:"), info[1].length());
    nframes = std::atoi(info[2].substr(strlen("nframes:"), info[2].length()).c_str());
    anim_fps = std::atoi(info[3].substr(strlen("fps:"), info[3].length()).c_str());

    // Transfer frame info to frames vector
    for (int i = 4; i < info.size(); i++) {
        std::string id_string = info[i].substr(0, (find_char_begin(info[i], ':')));
        int id = std::atoi(id_string.c_str());

        std::string filename = info[i].substr(find_char_begin(info[i], ':') + 1, info[i].length());

        Frame fr = frame_new(id, filename);
        frames.push_back(fr);
    }

    comp_manager.read(info_comp);

    return 0;
}

void State::write(void)
{
    // Make directories for project
    std::filesystem::create_directories(project_dirname + "frames/");

    // Create necessary files:
    // project.fbfp file
    std::string project_at = project_dirname + "project.fbfp";
    FILE *f = fopen(project_at.c_str(), "w");

    // Write to project.fbfp
    fprintf(f, "fbf\n");
    fprintf(f, "title:%s\n", anim_title.c_str());
    fprintf(f, "nframes:%d\n", nframes);
    fprintf(f, "fps:%d\n", anim_fps);
    
    for (int i = 0; i < nframes; i++) {
        fprintf(f, "%d:%s\n", frames[i].id, frames[i].img_path.c_str());
    }

    comp_manager.write();

    fclose(f);
}

void State::end(void)
{
    for (int i = 0; i < nframes; i++) {
        frames[i].end();
    }
    copied_frame.end();
}

// File-specific functions
// -----------------------

int find_char_begin(std::string s, char c)
{
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == c)
            return i;
    }

    return -1;
}

std::string replace_all(std::string s, char c1, char c2)
{
    std::string snew = s;
    for (int i = 0; i < s.length(); i++) {
        if (snew[i] == c1) {
            snew[i] = c2;
        }
    }

    return snew;
}

std::string lowercase(std::string s)
{
    std::string snew = s;
    for (int i = 0; i < s.length(); i++) {
        snew[i] = tolower(snew[i]);
    }

    return snew;
}

