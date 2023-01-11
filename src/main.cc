#include <stdio.h>
#include <string>
#include <map>
#include "config.h"
#include "raylib.h"
#include "ui.h"
#include "raygui.h"
#include <iostream>

int main()
{
    SetTraceLogLevel(LOG_NONE);
    InitWindow(WIDTH, HEIGHT, TITLE);

    Font font = LoadFont("assets/JandaManateeSolid.ttf");
    GuiSetFont(font);

    State state = state_new();
    UI ui = ui_new(&state);
    ui.font = font;

    Texture fbf_logo = LoadTexture("assets/logo.png");

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

        // Draw logo and version
        DrawTexture(fbf_logo, GetScreenWidth() - 200, 15, WHITE);
        DrawTextEx(font, VERSION, Vector2{ float(GetScreenWidth() - 70), float(GetScreenHeight() - 35) }, 32.0f, 2.0f, BLACK);

        EndDrawing();
    }

    UnloadFont(font);
    UnloadTexture(fbf_logo);
    ui.end();
    state.end();

    CloseWindow();
    return 0;
}

