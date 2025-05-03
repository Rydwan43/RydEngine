#pragma once
#include <entt.hpp>
#include <raylib.h>
#include <raymath.h>
#include "../Entities/Components/TransformComponent.hpp"
#include "../Entities/Components/ModelComponent.hpp"

class RenderSystem
{
public:
    static void Render(entt::registry &registry)
    {
        auto view = registry.view<TransformComponent, ModelComponent>();
        for (auto entity : view)
        {
            auto &transform = view.get<TransformComponent>(entity);
            auto &modelComp = view.get<ModelComponent>(entity);

            Matrix transformMatrix = MatrixMultiply(
                MatrixMultiply(
                    MatrixTranslate(transform.position.x, transform.position.y, transform.position.z),
                    MatrixRotateXYZ(Vector3Scale(transform.rotation, DEG2RAD))),
                MatrixScale(transform.scale.x, transform.scale.y, transform.scale.z));

            DrawModelEx(modelComp.model, transform.position, {0, 1, 0}, transform.rotation.y, transform.scale, modelComp.tint);
        }
    }
};
