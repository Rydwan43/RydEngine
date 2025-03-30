// #include "Core/Settings.hpp"
// #include "Core/ImGuiManager.hpp"
// #include "Core/UIManager.hpp"
// #include "Entities/RCamera.hpp"
// #include "Entities/Primitives/Cube.hpp"
// #include "Entities/Primitives/Sphere.hpp"
// #include <memory>
// #include <vector>
// #include "Core/EngineState.hpp"
// #include "Core/MenuManager.hpp"

// int main()
// {
//     Settings settings;
//     InitWindow(settings.resolutionX, settings.resolutionY, settings.title.c_str());
//     SetExitKey(0); // Disable automatic exit with escape key
//     settings.ApplySettings();

//     ImGuiManager imGui;
//     auto camera = std::make_unique<RCamera>(Vector3{0, 2, 5}, Vector3{0, 0, 0});
//     std::vector<std::unique_ptr<GameObject>> entities;

//     ShapeParams shapeParams;
//     UIManager ui(shapeParams, entities);

//     auto quitCallback = []()
//     { CloseWindow(); };
//     MenuManager menu(settings, quitCallback);

//     EngineState engineState;

//     while (!WindowShouldClose())
//     {
//         // Only process game input if menu is not visible
//         if (!menu.IsMenuVisible())
//         {
//             // Handle mode switching
//             if (IsKeyPressed(KEY_TAB))
//             {
//                 engineState.mode = (engineState.mode == EngineMode::Edit) ? EngineMode::Play : EngineMode::Edit;
//                 engineState.justSwitched = true;
//             }
//             else
//             {
//                 engineState.justSwitched = false;
//             }

//             camera->Update(engineState, entities);

//             // Only allow entity creation in edit mode
//             if (engineState.mode == EngineMode::Edit)
//             {
//                 if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !ImGui::GetIO().WantCaptureMouse)
//                 {
//                     Ray ray = GetMouseRay(GetMousePosition(), *camera->GetCamera());
//                     Vector3 spawnPos = Vector3Add(ray.position, Vector3Scale(ray.direction, 10.0f));

//                     switch (shapeParams.currentShape)
//                     {
//                     case ShapeType::Cube:
//                         entities.push_back(std::make_unique<Cube>(spawnPos, shapeParams.cubeSize, shapeParams.shapeColor));
//                         break;
//                     case ShapeType::Sphere:
//                         entities.push_back(std::make_unique<Sphere>(spawnPos, shapeParams.sphereRadius,
//                                                                     shapeParams.sphereSegments, shapeParams.shapeColor));
//                         break;
//                     }
//                 }
//             }
//         }

//         float deltaTime = GetFrameTime();

//         BeginDrawing();
//         ClearBackground(RAYWHITE);

//         BeginMode3D(*camera->GetCamera());
//         camera->Update(engineState, entities);
//         DrawGrid(10, 1.0f);

//         for (const auto &entity : entities)
//         {
//             entity->Draw();
//         }
//         EndMode3D();

//         imGui.BeginFrame();

//         // Draw game UI only if menu is not visible
//         if (!menu.IsMenuVisible() && engineState.mode == EngineMode::Edit)
//         {
//             ui.Draw();
//         }

//         menu.Update();

//         imGui.EndFrame();

//         DrawFPS(10, 10);
//         EndDrawing();
//     }

//     CloseWindow();
//     return 0;
// }

#include "raylib.h"
#include "raymath.h"

#include "imgui.h"
#include "rlImGui.h"
#include "rlImGuiColors.h"

#include "Core/ImGuiTheme.hpp"

#include "Views/DocumentWindow.hpp"
#include "Views/ImageViewerWindow.hpp"
#include "Views/SceneWindow.hpp"

#include "Helpers/DpiScaling.hpp"

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
            ImGui::MenuItem("3D View", nullptr, &SceneView.Open);

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
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "RydEngine 0.0.1");
    SetExitKey(0);
    SetTargetFPS(30);
    rlImGuiSetup(true);
    ImGuiTheme::SetupImGuiStyle();
    ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

    ImageViewer.Setup();
    ImageViewer.Open = true;

    SceneView.Setup();
    SceneView.Open = true;

#ifdef IMGUI_HAS_DOCK
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif

    // Main game loop
    while (!WindowShouldClose() && !Quit) // Detect window close button or ESC key
    {
        ImageViewer.Update();
        SceneView.Update();

        BeginDrawing();
        ClearBackground(DARKGRAY);

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
            SceneView.Show();

        rlImGuiEnd();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    rlImGuiShutdown();

    ImageViewer.Shutdown();
    SceneView.Shutdown();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}