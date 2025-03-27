#pragma once
#include "../Core/Entity.hpp"
#include <cmath>

class RCamera : public Entity
{
private:
    Camera3D camera;
    Vector3 velocity;
    float moveSpeed;
    float mouseSensitivity;

public:
    RCamera(Vector3 pos, Vector3 target) : moveSpeed(10.0f),
                                           mouseSensitivity(0.05f),
                                           velocity({0, 0, 0})
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

    void Update()
    {
        Movement();
        UpdateCameraVectors();
    }

    void Movement()
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && !ImGui::GetIO().WantCaptureMouse)
        {
            SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);
            Vector2 mouseDelta = GetMouseDelta();
            rotation.y += mouseDelta.x * mouseSensitivity;
            rotation.x -= mouseDelta.y * mouseSensitivity;

            if (rotation.x > 89.0f)
                rotation.x = 89.0f;
            if (rotation.x < -89.0f)
                rotation.x = -89.0f;
        }
        else
        {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        float cosPitch = cosf(rotation.x * DEG2RAD);
        Vector3 forward = {
            cosf(rotation.y * DEG2RAD) * cosPitch,
            sinf(rotation.x * DEG2RAD),
            sinf(rotation.y * DEG2RAD) * cosPitch};
        Vector3 right = Vector3CrossProduct(forward, (Vector3){0, 1, 0});
        Vector3 up = {0, 1, 0};

        Vector3 input = {0, 0, 0};

        if (IsKeyDown(KEY_W))
            input = Vector3Add(input, forward);
        if (IsKeyDown(KEY_S))
            input = Vector3Subtract(input, forward);
        if (IsKeyDown(KEY_D))
            input = Vector3Add(input, right);
        if (IsKeyDown(KEY_A))
            input = Vector3Subtract(input, right);
        if (IsKeyDown(KEY_SPACE))
            input.y += 1.0f;
        if (IsKeyDown(KEY_LEFT_SHIFT))
            input.y -= 1.0f;

        if (!Vector3Equals(input, (Vector3){0, 0, 0}))
        {
            input = Vector3Normalize(input);
            input = Vector3Scale(input, moveSpeed * GetFrameTime());
        }

        position = Vector3Add(position, input);
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
};