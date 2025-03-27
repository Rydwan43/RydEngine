#include "Core/Settings.hpp"
#include "Core/ImGuiManager.hpp"
#include "Entities/RCamera.hpp"
#include "Entities/Cube.hpp"
#include <memory>
#include <vector>

int main()
{
    Settings settings;
    InitWindow(settings.resolutionX, settings.resolutionY, settings.title.c_str());
    settings.ApplySettings();

    ImGuiManager imGui;
    auto camera = std::make_unique<RCamera>(Vector3{0, 2, 5}, Vector3{0, 0, 0});
    std::vector<std::unique_ptr<StaticBody>> entities;

    float cubeSize = 1.0f;
    Color cubeColor = RED;

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !ImGui::GetIO().WantCaptureMouse)
        {
            Ray ray = GetMouseRay(GetMousePosition(), *camera->GetCamera());
            Vector3 spawnPos = Vector3Add(ray.position, Vector3Scale(ray.direction, 10.0f));
            entities.push_back(std::make_unique<Cube>(spawnPos, cubeSize, cubeColor));
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(*camera->GetCamera());
        camera->Update();
        DrawGrid(10, 1.0f);

        for (const auto &entity : entities)
        {
            entity->Draw();
        }
        EndMode3D();

        // ImGui UI
        imGui.BeginFrame();

        ImGui::Begin("Tools");
        ImGui::SliderFloat("Cube Size", &cubeSize, 0.1f, 5.0f);
        float color[4] = {
            cubeColor.r / 255.0f,
            cubeColor.g / 255.0f,
            cubeColor.b / 255.0f,
            cubeColor.a / 255.0f};
        if (ImGui::ColorEdit4("Cube Color", color))
        {
            cubeColor = Color{
                (unsigned char)(color[0] * 255),
                (unsigned char)(color[1] * 255),
                (unsigned char)(color[2] * 255),
                (unsigned char)(color[3] * 255)};
        }
        ImGui::Text("Entities: %zu", entities.size());
        if (ImGui::Button("Clear All"))
        {
            entities.clear();
        }
        ImGui::End();

        imGui.EndFrame();

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}