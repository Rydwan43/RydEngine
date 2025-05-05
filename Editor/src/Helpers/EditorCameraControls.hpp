#pragma once
#include "raylib.h"
#include "raymath.h"

class EditorCameraControls
{
public:
    Camera camera{};
    Vector3 position = {5.0f, 5.0f, 5.0f};
    Vector2 rotation = {0.0f, 0.0f};

    EditorCameraControls()
    {
        camera.fovy = 60.0f;
        camera.projection = CAMERA_PERSPECTIVE;
        camera.up = {0, 1, 0};
    }

    void Update(float deltaTime)
    {
        float moveSpeed = 10.0f * deltaTime;
        float rotSpeed = 0.1f;

        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
        {
            Vector2 mouse = GetMouseDelta();
            rotation.y += mouse.x * rotSpeed;
            rotation.x -= mouse.y * rotSpeed;
            rotation.x = Clamp(rotation.x, -89.0f, 89.0f);

            Vector3 forward = {
                cosf(DEG2RAD * rotation.x) * sinf(DEG2RAD * rotation.y),
                sinf(DEG2RAD * -rotation.x),
                cosf(DEG2RAD * rotation.x) * cosf(DEG2RAD * rotation.y)};
            forward = Vector3Normalize(forward);

            Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, {0, 1, 0}));

            if (IsKeyDown(KEY_W))
                position = Vector3Add(position, Vector3Scale(forward, moveSpeed));
            if (IsKeyDown(KEY_S))
                position = Vector3Subtract(position, Vector3Scale(forward, moveSpeed));
            if (IsKeyDown(KEY_A))
                position = Vector3Subtract(position, Vector3Scale(right, moveSpeed));
            if (IsKeyDown(KEY_D))
                position = Vector3Add(position, Vector3Scale(right, moveSpeed));

            camera.position = position;
            camera.target = Vector3Add(position, forward);
        }
    }

    const Camera &GetCamera() const { return camera; }
};
