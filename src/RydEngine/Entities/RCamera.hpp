#pragma once
#include "../Core/Entity.hpp"
#include "../Core/EngineState.hpp"
#include "imgui.h"
#include <cmath>

class RCamera : public Entity
{
private:
    Camera3D camera;
    Vector3 velocity;
    float moveSpeed;
    float mouseSensitivity;
    float jumpForce = 8.0f;
    float gravity = 20.0f;
    bool isGrounded = false;

    // Store edit mode transform
    Vector3 editPosition;
    Vector3 editRotation;

public:
    RCamera(Vector3 pos, Vector3 target) : moveSpeed(10.0f),
                                           mouseSensitivity(0.05f),
                                           velocity({0, 0, 0}),
                                           editPosition(pos),
                                           editRotation({0, 0, 0})
    {
        position = pos;
        camera = {
            .position = pos,
            .target = target,
            .up = {0.0f, 1.0f, 0.0f},
            .fovy = 90.0f,
            .projection = CAMERA_PERSPECTIVE};
    }

    virtual ~RCamera() noexcept override = default;

    void Update(const EngineState &state, const std::vector<std::unique_ptr<StaticBody>> &entities)
    {
        if (state.justSwitched)
        {
            if (state.mode == EngineMode::Edit)
            {
                // Restore edit mode position and show cursor
                position = editPosition;
                rotation = editRotation;
                EnableCursor();
            }
            else
            {
                // Store current position before entering play mode and hide cursor
                editPosition = position;
                editRotation = rotation;
                DisableCursor();
            }
        }

        if (state.mode == EngineMode::Edit)
        {
            EditModeUpdate();
        }
        else
        {
            PlayModeUpdate(entities);
        }

        UpdateCameraVectors();
    }

    void UpdateCameraVectors()
    {
        float cosPitch = cosf(rotation.x * DEG2RAD);
        Vector3 direction = {
            cosf(rotation.y * DEG2RAD) * cosPitch,
            sinf(rotation.x * DEG2RAD),
            sinf(rotation.y * DEG2RAD) * cosPitch};

        camera.position = position;
        camera.target = Vector3Add(position, direction);
    }

    Camera3D *GetCamera() { return &camera; }

private:
    void EditModeUpdate()
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);
            Vector2 mouseDelta = GetMouseDelta();
            rotation.y += mouseDelta.x * mouseSensitivity;
            rotation.x -= mouseDelta.y * mouseSensitivity;

            if (rotation.x > 89.0f)
                rotation.x = 89.0f;
            if (rotation.x < -89.0f)
                rotation.x = -89.0f;

            // Movement in edit mode when right mouse is held
            float cosPitch = cosf(rotation.x * DEG2RAD);
            Vector3 forward = {
                cosf(rotation.y * DEG2RAD) * cosPitch,
                sinf(rotation.x * DEG2RAD),
                sinf(rotation.y * DEG2RAD) * cosPitch};
            Vector3 right = Vector3CrossProduct(forward, {0, 1, 0});
            Vector3 up = {0, 1, 0};

            Vector3 moveDir = {0, 0, 0};
            if (IsKeyDown(KEY_W))
                moveDir = Vector3Add(moveDir, forward);
            if (IsKeyDown(KEY_S))
                moveDir = Vector3Subtract(moveDir, forward);
            if (IsKeyDown(KEY_D))
                moveDir = Vector3Add(moveDir, right);
            if (IsKeyDown(KEY_A))
                moveDir = Vector3Subtract(moveDir, right);
            if (IsKeyDown(KEY_SPACE))
                moveDir.y += 1.0f;
            if (IsKeyDown(KEY_LEFT_SHIFT))
                moveDir.y -= 1.0f;

            if (!Vector3Equals(moveDir, {0, 0, 0}))
            {
                moveDir = Vector3Normalize(moveDir);
                moveDir = Vector3Scale(moveDir, moveSpeed * GetFrameTime());
                position = Vector3Add(position, moveDir);
            }
        }
        else
        {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }
    }

    void PlayModeUpdate(const std::vector<std::unique_ptr<StaticBody>> &entities)
    {
        // Mouse look (always active in play mode)
        Vector2 mouseDelta = GetMouseDelta();
        rotation.y += mouseDelta.x * mouseSensitivity;
        rotation.x -= mouseDelta.y * mouseSensitivity;
        rotation.x = std::clamp(rotation.x, -89.0f, 89.0f);

        // Calculate movement direction
        float cosPitch = cosf(rotation.x * DEG2RAD);
        Vector3 forward = {
            cosf(rotation.y * DEG2RAD) * cosPitch,
            0, // No vertical component for movement
            sinf(rotation.y * DEG2RAD) * cosPitch};
        Vector3 right = Vector3CrossProduct(forward, {0, 1, 0});

        // Movement input
        Vector3 moveDir = {0, 0, 0};
        if (IsKeyDown(KEY_W))
            moveDir = Vector3Add(moveDir, forward);
        if (IsKeyDown(KEY_S))
            moveDir = Vector3Subtract(moveDir, forward);
        if (IsKeyDown(KEY_D))
            moveDir = Vector3Add(moveDir, right);
        if (IsKeyDown(KEY_A))
            moveDir = Vector3Subtract(moveDir, right);

        if (!Vector3Equals(moveDir, {0, 0, 0}))
        {
            moveDir = Vector3Normalize(moveDir);
            moveDir = Vector3Scale(moveDir, moveSpeed);
            velocity.x = moveDir.x;
            velocity.z = moveDir.z;
        }
        else
        {
            // Apply friction
            velocity.x *= 0.8f;
            velocity.z *= 0.8f;
        }

        // Jump
        if (IsKeyPressed(KEY_SPACE) && isGrounded)
        {
            velocity.y = jumpForce;
            isGrounded = false;
        }

        // Apply gravity
        if (!isGrounded)
        {
            velocity.y -= gravity * GetFrameTime();
        }

        // Move and check collisions
        Vector3 newPos = Vector3Add(position, Vector3Scale(velocity, GetFrameTime()));

        // Simple collision check (you might want to make this more sophisticated)
        isGrounded = false;
        for (const auto &entity : entities)
        {
            // Basic AABB collision (assuming all entities are cubes for simplicity)
            // You should implement proper collision detection based on your needs
            if (CheckCollisionWithEntity(newPos, entity.get()))
            {
                if (velocity.y < 0)
                    isGrounded = true;
                velocity.y = 0;
                return;
            }
        }

        position = newPos;
        if (position.y < 0)
        {
            position.y = 0;
            velocity.y = 0;
            isGrounded = true;
        }
    }

    bool CheckCollisionWithEntity(Vector3 pos, const StaticBody *entity)
    {
        // Implement proper collision detection here
        // This is a very basic example
        const float playerRadius = 0.5f;
        const float playerHeight = 2.0f;

        // Get entity bounds (you'll need to add this to StaticBody)
        BoundingBox entityBounds = entity->GetBounds();

        // Check collision
        return CheckCollisionBoxSphere(entityBounds,
                                       {pos.x, pos.y + playerHeight / 2, pos.z}, playerRadius);
    }
};