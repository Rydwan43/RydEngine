#include "raylib.h"
#include "raymath.h"

#include "imgui.h"
#include "rlImGui.h"
#include "rlImGuiColors.h"

#include "Game.hpp"

bool Quit = false;

int main(int argc, char *argv[])
{
    int screenWidth = 1600;
    int screenHeight = 900;

    // do not set the FLAG_WINDOW_HIGHDPI flag, that scales a low res framebuffer up to the native resolution.
    // use the native resolution and scale your geometry. FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT |
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "RydEngine 0.0.2 (Game)");
    SetExitKey(0);
    SetTargetFPS(60);
    rlImGuiSetup(true);
    ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

    Game game;
    game.Setup();

    while (!WindowShouldClose() && !Quit) // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        game.Update();
        game.Render();

        EndDrawing();
    }
    game.Shutdown();
    rlImGuiShutdown();
    CloseWindow();

    return 0;
}