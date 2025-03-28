#pragma once
#include "../Core/Entity.hpp"

class StaticBody : public Entity
{
protected:
    Color color;
    BoundingBox bounds;

public:
    StaticBody(Vector3 pos, Color col = WHITE) : color(col)
    {
        position = pos;
    }

    virtual void UpdateBounds() = 0;
    virtual BoundingBox GetBounds() const { return bounds; }
    virtual void Draw() = 0;
    virtual ~StaticBody() noexcept override = default;
};