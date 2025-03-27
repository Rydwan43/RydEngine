#pragma once
#include "StaticBody.hpp"

class Cube : public StaticBody
{
private:
    float size;

public:
    Cube(Vector3 pos, float cubeSize = 1.0f, Color col = WHITE)
        : StaticBody(pos, col), size(cubeSize) {}

    void Draw() override
    {
        DrawCube(position, size, size, size, color);
        DrawCubeWires(position, size, size, size, BLACK);
    }
};