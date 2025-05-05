#pragma once
#include "DocumentWindow.hpp"
#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include <entt.hpp>
#include <Entities/Scene.hpp>
#include <Entities/Components/TransformComponent.hpp>
#include <Entities/Components/ModelComponent.hpp>
#include <Entities/Components/PlayerComponent.hpp>
#include <Entities/Components/CameraComponent.hpp>
#include <Entities/Components/NameComponent.hpp>
#include <Rendering/RenderSystem.hpp>
#include "Components/Inspector.hpp"
#include "Components/EntityList.hpp"
#include "Helpers/EditorCameraControls.hpp"
#include "../../../Game/src/Game.hpp"

class SceneViewWindow : public DocumentWindow
{
private:
    Scene scene;
    Game game;
    bool isPlaying = false;
    entt::entity selectedEntity = entt::null;
    EditorCameraControls editorCam;

public:
    SceneViewWindow() {}

    void Setup() override
    {
        game.Setup();
        auto cube = scene.CreateEntity("Cube");
        scene.registry.emplace<TransformComponent>(cube, TransformComponent{});
        Model cubeModel = LoadModelFromMesh(GenMeshCube(1, 1, 1));
        scene.registry.emplace<ModelComponent>(cube, ModelComponent{cubeModel});

        // Player
        auto player = scene.CreateEntity("Player");
        scene.registry.emplace<TransformComponent>(player, TransformComponent{});
        scene.registry.emplace<PlayerComponent>(player, PlayerComponent{});
        scene.registry.emplace<CameraComponent>(player, CameraComponent{});
        scene.registry.emplace<ModelComponent>(player, ModelComponent{cubeModel});
    }

    void Update() override
    {
        float delta = GetFrameTime();

        if (IsKeyPressed(KEY_TAB))
        {
            isPlaying = !isPlaying;
            if (isPlaying)
            {
                HideCursor();
            }

            if (!isPlaying)
            {
                ShowCursor();
            }
        }

        if (isPlaying)
        {
            game.Update();
        }
        else
        {
            editorCam.Update(delta);
        }
    }

    void Show() override
    {
        if (!Open)
            return;

        if (isPlaying)
        {
            game.Render();
        }
        else
        {
            BeginMode3D(editorCam.GetCamera());
            RenderSystem::Render(scene.registry);
            DrawGrid(100, 1.0f);
            EndMode3D();

            EntityList::Show(scene, selectedEntity);
            Inspector::Show(scene, selectedEntity);
        }
    }

    void Shutdown() override
    {
        auto view = scene.registry.view<ModelComponent>();
        for (auto entity : view)
        {
            auto &modelComp = view.get<ModelComponent>(entity);
            UnloadModel(modelComp.model);
        }
        scene.registry.clear();
        game.Shutdown();
    }
};
