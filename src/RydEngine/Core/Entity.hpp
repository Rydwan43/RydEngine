#pragma once
#include <raylib.h>
#include <raymath.h>

class Entity
{
protected:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;

public:
    Entity() : position({0, 0, 0}), rotation({0, 0, 0}), scale({1, 1, 1}) {}
    virtual ~Entity() noexcept = default;

    Vector3 GetPosition() const { return position; }
    Vector3 GetRotation() const { return rotation; }
    Vector3 GetScale() const { return scale; }

    virtual void SetPosition(Vector3 pos) { position = pos; }
    void SetRotation(Vector3 rot) { rotation = rot; }
    void SetScale(Vector3 scl) { scale = scl; }
};