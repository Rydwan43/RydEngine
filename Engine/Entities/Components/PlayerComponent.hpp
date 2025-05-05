#pragma once
#include <raylib.h>

struct PlayerComponent
{
    float moveSpeed = 5.0f;
    float mouseSensitivity = 0.1f;
    float yaw = 0.0f;   // Horizontal rotation
    float pitch = 0.0f; // Vertical rotation
    float onGround = true;
    Vector3 velocity = {0.0f, 0.0f, 0.0f};
};
