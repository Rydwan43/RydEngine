#pragma once
#include "StaticBody.hpp"

class Cube : public StaticBody
{
private:
    float size;

public:
    Cube(Vector3 pos, float cubeSize = 1.0f, Color col = WHITE)
        : StaticBody(pos, col), size(cubeSize)
    {
        UpdateBounds();
    }

    void UpdateBounds() override
    {
        float halfSize = size / 2.0f;
        bounds = {
            {position.x - halfSize, position.y - halfSize, position.z - halfSize},
            {position.x + halfSize, position.y + halfSize, position.z + halfSize}};
    }

    void Draw() override
    {
        DrawCube(position, size, size, size, color);
        DrawCubeWires(position, size, size, size, BLACK);
    }

    void SetPosition(Vector3 pos) override
    {
        position = pos;
        UpdateBounds();
    }
};