#pragma once
#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include <entt.hpp>
#include <Entities/Scene.hpp>
#include <Entities/Components/TransformComponent.hpp>
#include <Entities/Components/ModelComponent.hpp>
#include <Entities/Components/PlayerComponent.hpp>
#include <Entities/Components/CameraComponent.hpp>
#include <Rendering/RenderSystem.hpp>
#include "Systems/PlayerSystem.hpp"

class Game
{
public:
    Scene scene;
    Camera *activeCamera = nullptr;
    Game() {}

    void Setup()
    {
        auto cube = scene.CreateEntity("Cube");
        scene.registry.emplace<TransformComponent>(cube, TransformComponent{});
        Model cubeModel = LoadModelFromMesh(GenMeshCube(1, 1, 1));
        scene.registry.emplace<ModelComponent>(cube, ModelComponent{cubeModel});

        auto player = scene.CreateEntity("Player");
        scene.registry.emplace<TransformComponent>(player, TransformComponent{});
        scene.registry.emplace<PlayerComponent>(player, PlayerComponent{});
        scene.registry.emplace<CameraComponent>(player, CameraComponent{});
        scene.registry.emplace<ModelComponent>(player, ModelComponent{cubeModel});

        auto view = scene.registry.view<PlayerComponent, CameraComponent>();
        for (auto entity : view)
        {
            auto &camComp = view.get<CameraComponent>(entity);
            activeCamera = &camComp.camera;
            break;
        }
    }

    void Update()
    {
        auto view = scene.registry.view<CameraComponent, TransformComponent>(entt::exclude<PlayerComponent>);
        for (auto entity : view)
        {
            auto &cam = view.get<CameraComponent>(entity).camera;
            auto &transform = view.get<TransformComponent>(entity);

            float speed = 10.0f * GetFrameTime();
            if (IsKeyDown(KEY_W))
                transform.position.z -= speed;
            if (IsKeyDown(KEY_S))
                transform.position.z += speed;
            if (IsKeyDown(KEY_A))
                transform.position.x -= speed;
            if (IsKeyDown(KEY_D))
                transform.position.x += speed;

            cam.position = transform.position;
            cam.target = Vector3Add(transform.position, {0, 0, -1});
        }

        PlayerSystem::Update(scene.registry, GetFrameTime());
    }

    void Render()
    {
        if (activeCamera)
        {
            BeginMode3D(*activeCamera);
            RenderSystem::Render(scene.registry);
            DrawGrid(100, 4.0f);
            EndMode3D();
        }
    }

    void Shutdown()
    {
        auto view = scene.registry.view<ModelComponent>();
        for (auto entity : view)
        {
            auto &modelComp = view.get<ModelComponent>(entity);
            UnloadModel(modelComp.model);
        }
    }
};
