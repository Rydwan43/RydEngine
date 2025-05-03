#pragma once
#include "DocumentWindow.hpp"
#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include <entt.hpp>
#include "../Entities/Scene.hpp"
#include "../Entities/Components/ModelComponent.hpp"
#include "../Entities/Components/TransformComponent.hpp"
#include "../Entities/Components/CameraComponent.hpp"
#include "../Entities/Components/PlayerComponent.hpp"
#include "../Systems/RenderSystem.hpp"
#include "../Systems/PlayerSystem.hpp"

class SceneViewWindow : public DocumentWindow
{
private:
    Scene scene;
    entt::entity selectedEntity = entt::null;

public:
    SceneViewWindow() {}

    void Setup() override
    {
        auto entity = scene.CreateEntity();
        scene.registry.emplace<TransformComponent>(entity, TransformComponent{});
        Model cubeModel = LoadModelFromMesh(GenMeshCube(1, 1, 1));
        scene.registry.emplace<ModelComponent>(entity, ModelComponent{cubeModel});

        auto player = scene.CreateEntity();
        scene.registry.emplace<TransformComponent>(player, TransformComponent{});
        scene.registry.emplace<PlayerComponent>(player, PlayerComponent{});
        scene.registry.emplace<CameraComponent>(player, CameraComponent{});
        scene.registry.emplace<ModelComponent>(player, ModelComponent{cubeModel});
    }

    void Update() override
    {
    }

    void Show() override
    {
        if (!Open)
            return;

        float delta = GetFrameTime();
        PlayerSystem::Update(scene.registry, delta);

        auto player = scene.registry.view<PlayerComponent>().front();
        auto &cam = scene.registry.get<CameraComponent>(player).camera;
        BeginMode3D(cam);
        RenderSystem::Render(scene.registry);
        DrawGrid(10, 1.0f);
        EndMode3D();

        ShowEntityList(scene, selectedEntity);
        ShowInspector(scene, selectedEntity);
    }

    void Shutdown() override
    {
        // Cleanup if needed
    }

    void ShowEntityList(Scene &scene, entt::entity &selected)
    {
        ImGui::Begin("Scene Hierarchy");

        auto view = scene.registry.view<entt::entity>();

        for (auto entity : view)
        {
            char label[64];
            snprintf(label, sizeof(label), "Entity %d", static_cast<int>(entity));

            if (ImGui::Selectable(label, selected == entity))
            {
                selected = entity;
            }
        }

        ImGui::End();
    }

    void ShowInspector(Scene &scene, entt::entity selected)
    {
        ImGui::Begin("Inspector");

        if (selected == entt::null)
        {
            ImGui::Text("No entity selected.");
            ImGui::End();
            return;
        }

        auto &registry = scene.registry;

        // TransformComponent UI
        if (registry.any_of<TransformComponent>(selected))
        {
            auto &transform = registry.get<TransformComponent>(selected);
            if (ImGui::CollapsingHeader("Transform"))
            {
                ImGui::DragFloat3("Position", &transform.position.x, 0.1f);
                ImGui::DragFloat3("Rotation", &transform.rotation.x, 1.0f);
                ImGui::DragFloat3("Scale", &transform.scale.x, 0.1f);
            }
        }

        // ModelComponent UI
        if (registry.any_of<ModelComponent>(selected))
        {
            auto &modelComp = registry.get<ModelComponent>(selected);
            if (ImGui::CollapsingHeader("Model"))
            {
                ImGui::ColorEdit4("Tint", (float *)&modelComp.tint);
            }
        }

        // PlayerComponent UI
        if (registry.any_of<PlayerComponent>(selected))
        {
            auto &player = registry.get<PlayerComponent>(selected);
            if (ImGui::CollapsingHeader("Player"))
            {
                ImGui::SliderFloat("Move Speed", &player.moveSpeed, 0.1f, 20.0f);
                ImGui::SliderFloat("Mouse Sensitivity", &player.mouseSensitivity, 0.01f, 1.0f);
            }
        }

        // CameraComponent UI
        if (registry.any_of<CameraComponent>(selected))
        {
            auto &cam = registry.get<CameraComponent>(selected);
            if (ImGui::CollapsingHeader("Camera"))
            {
                ImGui::DragFloat3("Offset", &cam.offset.x, 0.1f);
                ImGui::SliderFloat("FOV", &cam.camera.fovy, 10.0f, 120.0f);
            }
        }

        ImGui::End();
    }
};
