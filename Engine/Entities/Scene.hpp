#pragma once
#include <entt.hpp>
#include "Components/NameComponent.hpp"

class Scene
{
public:
    entt::registry registry;

    entt::entity CreateEntity(const std::string &name = "Entity")
    {
        entt::entity entity = registry.create();
        registry.emplace<NameComponent>(entity, NameComponent{name});
        return entity;
    }
};
