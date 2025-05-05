#pragma once
#include "imgui.h"
#include <entt.hpp>
#include <Entities/Scene.hpp>
#include <Entities/Components/TransformComponent.hpp>
#include <Entities/Components/ModelComponent.hpp>
#include <Entities/Components/PlayerComponent.hpp>
#include <Entities/Components/CameraComponent.hpp>
#include <Entities/Components/NameComponent.hpp>
#include <Rendering/RenderSystem.hpp>

class Inspector
{
public:
    static void Show(Scene &scene, entt::entity selected)
    {
        ImGui::Begin("Inspector");

        if (selected == entt::null)
        {
            ImGui::Text("No entity selected.");
            ImGui::End();
            return;
        }

        auto &registry = scene.registry;

        // NameComponent UI
        if (registry.any_of<NameComponent>(selected))
        {
            auto &nameComp = registry.get<NameComponent>(selected);
            char buffer[128];
            strncpy(buffer, nameComp.name.c_str(), sizeof(buffer));
            buffer[sizeof(buffer) - 1] = '\0';

            if (ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer)))
            {
                nameComp.name = std::string(buffer);
            }
        }

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
                float color[4] = {
                    modelComp.tint.r / 255.0f,
                    modelComp.tint.g / 255.0f,
                    modelComp.tint.b / 255.0f,
                    modelComp.tint.a / 255.0f};

                if (ImGui::ColorEdit4("Tint", color))
                {
                    modelComp.tint.r = (unsigned char)(color[0] * 255);
                    modelComp.tint.g = (unsigned char)(color[1] * 255);
                    modelComp.tint.b = (unsigned char)(color[2] * 255);
                    modelComp.tint.a = (unsigned char)(color[3] * 255);
                }
            }
        }

        // PlayerComponent UI
        if (registry.any_of<PlayerComponent>(selected))
        {
            auto &player = registry.get<PlayerComponent>(selected);
            if (ImGui::CollapsingHeader("Player"))
            {
                ImGui::SliderFloat("Move Speed", &player.moveSpeed, 0.1f, 20.0f);
                ImGui::SliderFloat("Mouse Sens", &player.mouseSensitivity, 0.01f, 1.0f);
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

        if (ImGui::CollapsingHeader("Add Component"))
        {
            if (!registry.any_of<TransformComponent>(selected))
            {
                if (ImGui::Button("Add Transform"))
                    registry.emplace<TransformComponent>(selected);
            }

            if (!registry.any_of<ModelComponent>(selected))
            {
                if (ImGui::Button("Add Model"))
                {
                    Model cubeModel = LoadModelFromMesh(GenMeshCube(1, 1, 1));
                    registry.emplace<ModelComponent>(selected, ModelComponent{cubeModel});
                }
            }

            if (!registry.any_of<PlayerComponent>(selected))
            {
                if (ImGui::Button("Add Player"))
                    registry.emplace<PlayerComponent>(selected);
            }

            if (!registry.any_of<CameraComponent>(selected))
            {
                if (ImGui::Button("Add Camera"))
                    registry.emplace<CameraComponent>(selected);
            }
        }

        ImGui::End();
    }
};