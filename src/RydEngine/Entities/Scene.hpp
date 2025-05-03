#pragma once
#include <entt.hpp>

class Scene
{
public:
    entt::registry registry;

    entt::entity CreateEntity()
    {
        return registry.create();
    }
};
