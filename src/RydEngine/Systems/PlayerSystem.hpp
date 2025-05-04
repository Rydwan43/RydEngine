#pragma once
#include <entt.hpp>
#include <raylib.h>
#include <raymath.h>
#include "../Entities/Components/TransformComponent.hpp"
#include "../Entities/Components/ModelComponent.hpp"
#include "../Entities/Components/PlayerComponent.hpp"
#include "../Entities/Components/CameraComponent.hpp"

class PlayerSystem
{
public:
    static void Update(entt::registry &registry, float deltaTime)
    {
        auto view = registry.view<TransformComponent, PlayerComponent, CameraComponent>();
        for (auto entity : view)
        {
            auto &transform = view.get<TransformComponent>(entity);
            auto &player = view.get<PlayerComponent>(entity);
            auto &camComp = view.get<CameraComponent>(entity);

            Vector2 mouseDelta = GetMouseDelta();
            if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
            {
                player.yaw -= mouseDelta.x * player.mouseSensitivity;
                player.pitch -= mouseDelta.y * player.mouseSensitivity;
                player.pitch = Clamp(player.pitch, -89.0f, 89.0f);
            }

            Vector3 front = {
                cosf(DEG2RAD * player.pitch) * sinf(DEG2RAD * player.yaw),
                sinf(DEG2RAD * player.pitch),
                cosf(DEG2RAD * player.pitch) * cosf(DEG2RAD * player.yaw)};
            front = Vector3Normalize(front);

            Vector3 right = Vector3Normalize(Vector3CrossProduct(front, {0.0f, 1.0f, 0.0f}));

            Vector3 velocity = {0};
            if (IsKeyDown(KEY_W))
                velocity = Vector3Add(velocity, front);
            if (IsKeyDown(KEY_S))
                velocity = Vector3Subtract(velocity, front);
            if (IsKeyDown(KEY_A))
                velocity = Vector3Subtract(velocity, right);
            if (IsKeyDown(KEY_D))
                velocity = Vector3Add(velocity, right);

            velocity = Vector3Normalize(velocity);
            velocity = Vector3Scale(velocity, player.moveSpeed * deltaTime);
            transform.position = Vector3Add(transform.position, velocity);

            camComp.camera.position = Vector3Add(transform.position, camComp.offset);
            camComp.camera.target = Vector3Add(camComp.camera.position, front);
        }
    }
};
