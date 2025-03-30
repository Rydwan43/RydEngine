#pragma once
#include <raylib.h>
#include <raymath.h>

class Entity
{
protected:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;

    Vector3 originalPosition;
    Vector3 originalRotation;
    Vector3 originalScale;

public:
    Entity() : position({0, 0, 0}), rotation({0, 0, 0}), scale({1, 1, 1})
    {
        originalPosition = position;
        originalRotation = rotation;
        originalScale = scale;
    }
    virtual ~Entity() noexcept = default;

    Vector3 GetPosition() const { return position; }
    Vector3 GetRotation() const { return rotation; }
    Vector3 GetScale() const { return scale; }

    virtual void Reset()
    {
        position = originalPosition;
        rotation = originalRotation;
        scale = originalScale;
    }

    virtual void SetPosition(Vector3 pos) { position = pos; }
    virtual void SetRotation(Vector3 rot) { rotation = rot; }
    virtual void SetScale(Vector3 scl) { scale = scl; }

    virtual void Update() {}
    virtual void UpdateEditMode() {}
    virtual void UpdatePlayMode() {}
    virtual void Draw() {}
};