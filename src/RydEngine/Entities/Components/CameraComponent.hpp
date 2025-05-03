#pragma once
#include "raylib.h"

struct CameraComponent
{
    Vector3 offset = {0.0f, 1.8f, 0.0f}; // Typical eye height
    Camera3D camera;

    CameraComponent()
    {
        camera.fovy = 45.0f;
        camera.projection = CAMERA_PERSPECTIVE;
        camera.up = {0.0f, 1.0f, 0.0f};
    }
};
