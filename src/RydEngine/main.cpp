#include "Core/Settings.hpp"
#include "Core/ImGuiManager.hpp"
#include "Core/UIManager.hpp"
#include "Entities/RCamera.hpp"
#include "Entities/Cube.hpp"
#include "Entities/Sphere.hpp"
#include <memory>
#include <vector>
#include "Core/EngineState.hpp"
#include "Core/MenuManager.hpp"

int main()
{
    Settings settings;
    InitWindow(settings.resolutionX, settings.resolutionY, settings.title.c_str());
    SetExitKey(0); // Disable automatic exit with escape key
    settings.ApplySettings();

    ImGuiManager imGui;
    auto camera = std::make_unique<RCamera>(Vector3{0, 2, 5}, Vector3{0, 0, 0});
    std::vector<std::unique_ptr<StaticBody>> entities;

    ShapeParams shapeParams;
    UIManager ui(shapeParams, entities);

    auto quitCallback = []()
    { CloseWindow(); };
    MenuManager menu(settings, quitCallback);

    EngineState engineState;

    while (!WindowShouldClose())
    {
        // Only process game input if menu is not visible
        if (!menu.IsMenuVisible())
        {
            // Handle mode switching
            if (IsKeyPressed(KEY_TAB))
            {
                engineState.mode = (engineState.mode == EngineMode::Edit) ? EngineMode::Play : EngineMode::Edit;
                engineState.justSwitched = true;
            }
            else
            {
                engineState.justSwitched = false;
            }

            camera->Update(engineState, entities);

            // Only allow entity creation in edit mode
            if (engineState.mode == EngineMode::Edit)
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !ImGui::GetIO().WantCaptureMouse)
                {
                    Ray ray = GetMouseRay(GetMousePosition(), *camera->GetCamera());
                    Vector3 spawnPos = Vector3Add(ray.position, Vector3Scale(ray.direction, 10.0f));

                    switch (shapeParams.currentShape)
                    {
                    case ShapeType::Cube:
                        entities.push_back(std::make_unique<Cube>(spawnPos, shapeParams.cubeSize, shapeParams.shapeColor));
                        break;
                    case ShapeType::Sphere:
                        entities.push_back(std::make_unique<Sphere>(spawnPos, shapeParams.sphereRadius,
                                                                    shapeParams.sphereSegments, shapeParams.shapeColor));
                        break;
                    }
                }
            }
        }

        float deltaTime = GetFrameTime();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(*camera->GetCamera());
        camera->Update(engineState, entities);
        DrawGrid(10, 1.0f);

        for (const auto &entity : entities)
        {
            entity->Draw();
        }
        EndMode3D();

        imGui.BeginFrame();

        // Draw game UI only if menu is not visible
        if (!menu.IsMenuVisible() && engineState.mode == EngineMode::Edit)
        {
            ui.Draw();
        }

        // Always draw menu (it handles its own visibility)
        menu.Update();

        imGui.EndFrame();

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}