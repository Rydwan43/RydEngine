#pragma once
#include <Entities/Scene.hpp>
#include "imgui.h"
#include <entt.hpp>

class EntityList
{

public:
    static void Show(Scene &scene, entt::entity &selected)
    {
        ImGui::Begin("Scene Hierarchy");

        auto &registry = scene.registry;
        auto view = registry.view<NameComponent>();

        entt::entity entityToDelete = entt::null;

        for (auto entity : view)
        {
            auto &name = view.get<NameComponent>(entity).name;
            std::string label = name + "##" + std::to_string((uint32_t)entity);

            bool isSelected = (selected == entity);
            if (ImGui::Selectable(label.c_str(), isSelected))
            {
                selected = entity;
            }

            if (ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("Delete"))
                {
                    entityToDelete = entity;
                }
                ImGui::EndPopup();
            }
        }

        if (entityToDelete != entt::null)
        {
            if (selected == entityToDelete)
                selected = entt::null;
            registry.destroy(entityToDelete);
        }

        static int entityCount = 0;
        if (ImGui::Button("Add Entity"))
        {
            std::string entityName = "Entity " + std::to_string(entityCount++);
            auto newEntity = scene.CreateEntity(entityName);
            registry.emplace<TransformComponent>(newEntity, TransformComponent{});
        }

        ImGui::End();
    }
};
