#pragma once
#include "GameObject.hpp"
#include "raymath.h"

class Player : public GameObject
{
private:
    Vector3 velocity = {0, 0, 0};
    bool isGrounded = false;
    float moveSpeed = 5.0f;
    float jumpForce = 1.0f;
    float friction = 6.0f;

public:
    Player(Vector3 pos) : GameObject("Player", pos, {1, 2, 1}) {}

    void Update(float deltaTime) override
    {
        Vector3 forward = {0, 0, 1};
        Vector3 right = {1, 0, 0};

        if (IsKeyDown(KEY_W))
            velocity = Vector3Add(velocity, Vector3Scale(forward, moveSpeed * deltaTime));
        if (IsKeyDown(KEY_S))
            velocity = Vector3Add(velocity, Vector3Scale(forward, -moveSpeed * deltaTime));
        if (IsKeyDown(KEY_D))
            velocity = Vector3Add(velocity, Vector3Scale(right, -moveSpeed * deltaTime));
        if (IsKeyDown(KEY_A))
            velocity = Vector3Add(velocity, Vector3Scale(right, moveSpeed * deltaTime));

        // controller support
        if (IsGamepadAvailable(0))
        {
            Vector2 move = {GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X), GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y)};
            if (Vector2Length(move) > 0.1f)
            {
                move = Vector2Normalize(move);
                move.x = -move.x;
                move.y = -move.y;
                velocity = Vector3Add(velocity, Vector3Scale({move.x, 0, move.y}, moveSpeed * deltaTime));
            }
        }

        if (isGrounded)
        {
            velocity.x *= (1.0f - friction * deltaTime);
            velocity.z *= (1.0f - friction * deltaTime);

            if (IsKeyPressed(KEY_SPACE))
            {
                velocity.y = jumpForce;
                isGrounded = false;
            }
        }
        else
        {
            velocity.y -= 9.8f * deltaTime;
        }

        localPosition = Vector3Add(localPosition, velocity);
        if (localPosition.y <= 1.0f)
        {
            localPosition.y = 1.0f;
            isGrounded = true;
            velocity.y = 0;
        }
    }

    void Draw() override
    {
        Vector3 globalPos = GetGlobalPosition();
        Vector3 localScale = GetGlobalScale();
        DrawCube(globalPos, localScale.x, localScale.y, localScale.z, BLUE);
    }
};
