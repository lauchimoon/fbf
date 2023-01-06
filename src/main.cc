#include <stdio.h>
#include <string>
#include <map>
#include "config.h"
#include "raylib.h"
#include "ui.h"
#include "raygui.h"
#include <iostream>

std::string read_file_to_str(const char *filename);
std::vector<std::string> split_str(std::string s, char find);

int main()
{
    SetTraceLogLevel(LOG_NONE);
    InitWindow(WIDTH, HEIGHT, TITLE);

    Font font = LoadFont("assets/JandaManateeSolid.ttf");
    GuiSetFont(font);

    State state = state_new();
    UI ui = ui_new();
    ui.font = font;

    Texture fbf_logo = LoadTexture("assets/logo.png");

    std::string s = read_file_to_str("project/components.fbfp");
    std::vector<std::string> info = split_str(s, '\n');

    state.comp_manager.read(info);
    std::cout << state.comp_manager.get("img1").name << std::endl;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Updating
        // --------
        ui.update(&state);

        if (state.current_frame <= 0) // Clamp
            state.current_frame = 0;
        if (state.nframes <= 1)
            state.nframes = 1;

        // Drawing
        // -------
        BeginDrawing();

        ClearBackground(WHITE);

        ui.draw(&state);

        // Draw logo
        DrawTexture(fbf_logo, GetScreenWidth() - 200, 15, WHITE);

        EndDrawing();
    }

    UnloadFont(font);
    UnloadTexture(fbf_logo);
    ui.end();
    state.end();

    CloseWindow();
    return 0;
}

