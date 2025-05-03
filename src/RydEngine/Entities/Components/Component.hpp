#pragma once
#include <string>

class GameObject;

class Component
{
protected:
    GameObject *owner;

public:
    Component(GameObject *owner) : owner(owner) {}
    virtual ~Component() {}

    virtual void Update(float deltaTime) {}
};
