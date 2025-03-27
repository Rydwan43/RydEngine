#pragma once
#include "../Core/Entity.hpp"

class StaticBody : public Entity
{
protected:
    Color color;

public:
    StaticBody(Vector3 pos, Color col = WHITE) : color(col)
    {
        position = pos;
    }

    virtual ~StaticBody() noexcept override = default;
    virtual void Draw() = 0;
};