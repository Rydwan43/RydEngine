#pragma once
#include "../Entities/Scene.hpp"
#include "../Entities/GameObjects/PlayerObject.hpp"
#include "../Entities/GameObjects/CameraObject.hpp"
#include "DocumentWindow.hpp"
#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"

class SceneViewWindow : public DocumentWindow
{
private:
    Scene scene;

public:
    SceneViewWindow() {}

    void Setup() override
    {
        auto player = std::make_unique<Player>(Vector3{0, 1, 0});
        auto camera = std::make_unique<CameraObject>(Vector3{0, 2, -5});

        // Parent the camera to the player
        camera->SetParent(player.get());

        scene.cameraObj = camera.get();
        scene.AddObject(std::move(player));
        scene.AddObject(std::move(camera));
    }

    void Update() override
    {
        scene.Update(GetFrameTime());
    }

    void Show() override
    {
        if (!Open)
            return;

        BeginMode3D(scene.GetActiveCamera());
        scene.Draw();
        DrawGrid(10, 1.0f);
        EndMode3D();

        scene.DrawUI();
    }

    void Shutdown() override
    {
        // Cleanup if needed
    }
};
