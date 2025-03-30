#pragma once
#include "../Core/Entity.hpp"

class GameObject : public Entity
{
protected:
    Color color;
    BoundingBox bounds;

public:
    GameObject(Vector3 pos, Color col = WHITE) : color(col)
    {
        position = pos;
    }

    virtual void UpdateBounds() = 0;
    virtual BoundingBox GetBounds() const { return bounds; }
    virtual void Draw() override = 0;
    virtual ~GameObject() noexcept override = default;
};