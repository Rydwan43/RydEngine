#pragma once
#include "../GameObject.hpp"

class Sphere : public GameObject
{
private:
    float radius;
    int segments;

public:
    Sphere(Vector3 pos, float rad = 1.0f, int segs = 16, Color col = WHITE)
        : GameObject(pos, col), radius(rad), segments(segs)
    {
        UpdateBounds();
    }

    void UpdateBounds() override
    {
        bounds = {
            {position.x - radius, position.y - radius, position.z - radius},
            {position.x + radius, position.y + radius, position.z + radius}};
    }

    void Draw() override
    {
        DrawSphere(position, radius, color);
        DrawSphereWires(position, radius, segments, segments, BLACK);
    }

    void SetPosition(Vector3 pos) override
    {
        position = pos;
        UpdateBounds();
    }
};