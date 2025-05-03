#pragma once
#include <vector>
#include <memory>
#include "../Components/Component.hpp"
#include "raylib.h"
#include "raymath.h"
#include "imgui.h"
#include <string>

class GameObject
{
protected:
    Vector3 localPosition;
    Vector3 localRotation;
    Vector3 localScale;
    GameObject *parent = nullptr;

public:
    std::string name;
    bool selected = false;
    std::vector<GameObject *> children;

    GameObject(std::string name, Vector3 pos, Vector3 sc = {1, 1, 1}, Vector3 rot = {0, 0, 0})
        : name(name), localPosition(pos), localScale(sc), localRotation(rot) {}

    virtual ~GameObject() {}

    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;

    Vector3 GetGlobalPosition()
    {
        if (parent)
            return Vector3Add(parent->GetGlobalPosition(), localPosition);
        return localPosition;
    }

    Vector3 GetGlobalRotation()
    {
        if (parent)
            return Vector3Add(parent->GetGlobalRotation(), localRotation);
        return localRotation;
    }

    Vector3 GetGlobalScale()
    {
        if (parent)
            return Vector3Add(parent->GetGlobalScale(), localScale);
        return localScale;
    }

    // **Set Parent**
    void SetParent(GameObject *newParent)
    {
        if (parent)
            parent->RemoveChild(this);
        parent = newParent;
        if (parent)
            parent->AddChild(this);
    }

    void AddChild(GameObject *child)
    {
        children.push_back(child);
        child->parent = this;
    }

    void RemoveChild(GameObject *child)
    {
        children.erase(std::remove(children.begin(), children.end(), child), children.end());
        child->parent = nullptr;
    }

    virtual void DrawUI()
    {
        DrawStandardUI();
    }

    void DrawStandardUI()
    {
        ImGui::Text("Name: %s", name.c_str());

        ImGui::BeginGroup();
        ImGui::Text("Position");
        ImGui::DragFloat("px", &localPosition.x, 0.1f);
        ImGui::DragFloat("py", &localPosition.y, 0.1f);
        ImGui::DragFloat("pz", &localPosition.z, 0.1f);
        ImGui::EndGroup();
        ImGui::NewLine();
        ImGui::BeginGroup();
        ImGui::Text("Rotation");
        ImGui::DragFloat("rx", &localRotation.x, 0.5f);
        ImGui::DragFloat("ry", &localRotation.y, 0.5f);
        ImGui::DragFloat("rz", &localRotation.z, 0.5f);
        ImGui::NewLine();
        ImGui::EndGroup();
        ImGui::BeginGroup();
        ImGui::Text("Scale");
        ImGui::DragFloat("sx", &localScale.x, 0.1f);
        ImGui::DragFloat("sy", &localScale.y, 0.1f);
        ImGui::DragFloat("sz", &localScale.z, 0.1f);
        ImGui::EndGroup();

        if (parent)
            ImGui::Text("Parent: %s", parent->name.c_str());
    }
};