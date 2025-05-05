#pragma once
#include <entt.hpp>
#include <raylib.h>
#include <raymath.h>
#include <Entities/Components/TransformComponent.hpp>
#include <Entities/Components/ModelComponent.hpp>
#include <Entities/Components/PlayerComponent.hpp>
#include <Entities/Components/CameraComponent.hpp>

class PlayerSystem
{
public:
    static void Update(entt::registry &registry, float deltaTime)
    {
        const float groundFriction = 5.0f;
        const float groundAccel = 120.0f;
        const float airAccel = 50.0f;
        const float maxGroundSpeed = 20.0f;
        const float maxAirSpeed = 4.0f;
        const float jumpSpeed = 10.0f;
        const float gravity = 10.0f;
        const float controllerLookSensitivity = 200.0f;

        auto view = registry.view<TransformComponent, PlayerComponent, CameraComponent>();
        for (auto entity : view)
        {
            auto &transform = view.get<TransformComponent>(entity);
            auto &player = view.get<PlayerComponent>(entity);
            auto &camComp = view.get<CameraComponent>(entity);

            // ----------- INPUT: Mouse or Controller Look -----------
            Vector2 mouseDelta = GetMouseDelta();
            float lookX = -mouseDelta.x * player.mouseSensitivity;
            float lookY = -mouseDelta.y * player.mouseSensitivity;

            if (IsGamepadAvailable(0))
            {
                const float lookDeadZone = 0.1f;

                float rx = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);
                float ry = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y);

                if (fabsf(rx) >= lookDeadZone)
                    lookX -= rx * controllerLookSensitivity * deltaTime;
                if (fabsf(ry) >= lookDeadZone)
                    lookY -= ry * controllerLookSensitivity * deltaTime;
            }

            player.yaw += lookX;
            player.pitch += lookY;
            player.pitch = Clamp(player.pitch, -89.0f, 89.0f);

            Vector3 front = {
                cosf(DEG2RAD * player.pitch) * sinf(DEG2RAD * player.yaw),
                sinf(DEG2RAD * player.pitch),
                cosf(DEG2RAD * player.pitch) * cosf(DEG2RAD * player.yaw)};
            front = Vector3Normalize(front);
            Vector3 right = Vector3Normalize(Vector3CrossProduct(front, {0.0f, 1.0f, 0.0f}));

            // ----------- INPUT: Keyboard + Controller Movement -----------
            Vector3 wishDir = {0};

            if (IsKeyDown(KEY_W))
                wishDir = Vector3Add(wishDir, front);
            if (IsKeyDown(KEY_S))
                wishDir = Vector3Subtract(wishDir, front);
            if (IsKeyDown(KEY_A))
                wishDir = Vector3Subtract(wishDir, right);
            if (IsKeyDown(KEY_D))
                wishDir = Vector3Add(wishDir, right);

            if (IsGamepadAvailable(0))
            {
                float lx = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
                float ly = -GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);

                if (fabsf(lx) > 0.1f || fabsf(ly) > 0.1f)
                {
                    wishDir = Vector3Add(wishDir, Vector3Scale(right, lx));
                    wishDir = Vector3Add(wishDir, Vector3Scale(front, ly));
                }
            }

            wishDir.y = 0;
            float wishSpeed = 0.0f;
            if (Vector3Length(wishDir) > 0.01f)
            {
                wishDir = Vector3Normalize(wishDir);
                wishSpeed = player.onGround ? maxGroundSpeed : maxAirSpeed;
            }

            // Acceleration
            if (player.onGround)
            {
                ApplyFriction(player.velocity, groundFriction, deltaTime);
                Accelerate(player.velocity, wishDir, groundAccel, wishSpeed, deltaTime);
            }
            else
            {
                Accelerate(player.velocity, wishDir, airAccel, wishSpeed, deltaTime);
                player.velocity.y -= gravity * deltaTime;
            }

            // ----------- INPUT: Jumping (Keyboard or Gamepad) -----------
            bool jumpPressed = IsKeyDown(KEY_SPACE);

            if (IsGamepadAvailable(0))
            {
                jumpPressed |= IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);        // A
                jumpPressed |= IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_TRIGGER_1);         // Left Bumper
                jumpPressed |= (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_TRIGGER) > 0.5f); // Left Trigger
            }
            if (player.onGround)
            {
                if (jumpPressed)
                {
                    player.velocity.y = jumpSpeed;
                    player.onGround = false;
                }
                else
                {
                    ApplyFriction(player.velocity, groundFriction, deltaTime);
                }
            }
            else
            {
                player.velocity.y -= gravity * deltaTime;
            }

            // Basic collision with floor
            float newY = transform.position.y + player.velocity.y * deltaTime;
            if (newY <= 0.0f)
            {
                newY = 0.0f;
                player.velocity.y = 0.0f;
                player.onGround = true;
            }

            // Move position
            transform.position = Vector3Add(transform.position, Vector3Scale(player.velocity, deltaTime));
            transform.position.y = newY;

            // Update camera
            camComp.camera.position = Vector3Add(transform.position, camComp.offset);
            camComp.camera.target = Vector3Add(camComp.camera.position, front);

            HideCursor();
            SetMousePosition((float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f);
        }
    }

private:
    static void Accelerate(Vector3 &velocity, const Vector3 &wishDir, float accel, float wishSpeed, float deltaTime)
    {
        float currentSpeed = Vector3DotProduct(velocity, wishDir);
        float addSpeed = wishSpeed - currentSpeed;
        if (addSpeed <= 0.0f)
            return;

        float accelSpeed = accel * deltaTime * wishSpeed;
        if (accelSpeed > addSpeed)
            accelSpeed = addSpeed;

        Vector3 accelVec = Vector3Scale(wishDir, accelSpeed);
        velocity = Vector3Add(velocity, accelVec);
    }

    static void ApplyFriction(Vector3 &velocity, float friction, float deltaTime)
    {
        Vector3 horizontalVel = velocity;
        horizontalVel.y = 0;
        float speed = Vector3Length(horizontalVel);
        if (speed < 0.01f)
            return;

        float drop = speed * friction * deltaTime;
        float newSpeed = speed - drop;
        if (newSpeed < 0)
            newSpeed = 0;

        if (speed > 0)
        {
            newSpeed /= speed;
            velocity.x *= newSpeed;
            velocity.z *= newSpeed;
        }
    }
};