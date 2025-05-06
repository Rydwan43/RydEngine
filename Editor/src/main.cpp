#include "raylib.h"
#include "raymath.h"

#include "imgui.h"
#include "rlImGui.h"
#include "rlImGuiColors.h"

#include "Views/DocumentWindow.hpp"
#include "Views/ImageViewerWindow.hpp"
#include "Views/SceneViewWindow.hpp"

#include "Helpers/DpiScaling.hpp"
#include "Helpers/ImGuiTheme.hpp"

bool Quit = false;

bool ImGuiDemoOpen = false;

ImageViewerWindow ImageViewer;
SceneViewWindow SceneView;

void DoMainMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
                Quit = true;

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window"))
        {
            ImGui::MenuItem("ImGui Demo", nullptr, &ImGuiDemoOpen);
            ImGui::MenuItem("Image Viewer", nullptr, &ImageViewer.Open);
            ImGui::MenuItem("Scene View", nullptr, &SceneView.Open);

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

int main(int argc, char *argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 1600;
    int screenHeight = 900;

    // do not set the FLAG_WINDOW_HIGHDPI flag, that scales a low res framebuffer up to the native resolution.
    // use the native resolution and scale your geometry. FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT |
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(screenWidth, screenHeight, "RydEngine 0.0.3");
    SetExitKey(0);
    SetTargetFPS(60);
    rlImGuiSetup(true);
    ImGuiTheme::SetupImGuiStyle();
    ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

    ImageViewer.Setup();
    ImageViewer.Open = false;

    SceneView.Setup();
    SceneView.Open = true;

#ifdef IMGUI_HAS_DOCK
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif

    // Main game loop
    while (!WindowShouldClose() && !Quit) // Detect window close button or ESC key
    {
        ImageViewer.Update();

        BeginDrawing();
        ClearBackground(DARKGRAY);
        // draw fps
        DrawText(TextFormat("FPS: %i", GetFPS()), 10, 30, 20, WHITE);
        DrawText(TextFormat("Window Size: %i x %i", GetScreenWidth(), GetScreenHeight()), 10, 50, 20, WHITE);

        rlImGuiBegin();
#ifdef IMGUI_HAS_DOCK
        ImGui::DockSpaceOverViewport(0, NULL, ImGuiDockNodeFlags_PassthruCentralNode); // set ImGuiDockNodeFlags_PassthruCentralNode so that we can see the raylib contents behind the dockspace
#endif
        DoMainMenu();

        if (ImGuiDemoOpen)
            ImGui::ShowDemoWindow(&ImGuiDemoOpen);

        if (ImageViewer.Open)
            ImageViewer.Show();

        if (SceneView.Open)
        {
            SceneView.Update();
            SceneView.Show();
        }

        rlImGuiEnd();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    rlImGuiShutdown();

    ImageViewer.Shutdown();
    SceneView.Shutdown();

    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}