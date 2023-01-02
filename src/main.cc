#include <stdio.h>
#include <string>
#include <map>
#include "config.h"
#include "raylib.h"
#include "ui.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main()
{
    SetTraceLogLevel(LOG_NONE);
    InitWindow(WIDTH, HEIGHT, TITLE);

    Font font = LoadFont("assets/JandaManateeSolid.ttf");
    GuiSetFont(font);

    State state = state_new();
    UI ui = ui_new();
    ui.font = font;

    Color brush_color = BLACK;
    Texture fbf_logo = LoadTexture("assets/logo.png");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Updating
        // --------
        ui.update(&state);

        state.animtitle = ui.box_title.text;

        if (state.current_frame <= 0) // Clamp
            state.current_frame = 0;
        if (state.nframes <= 1)
            state.nframes = 1;

        // Drawing
        // -------
        BeginDrawing();

        ClearBackground(WHITE);

        ui.draw(state);

        // Draw logo
        DrawTexture(fbf_logo, GetScreenWidth() - 200, 15, WHITE);

        EndDrawing();
    }

    UnloadFont(font);
    UnloadTexture(fbf_logo);
    CloseWindow();
    return 0;
}
