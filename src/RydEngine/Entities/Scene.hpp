#pragma once
#include "GameObjects/GameObject.hpp"
#include "GameObjects/PlayerObject.hpp"
#include "GameObjects/CameraObject.hpp"
#include <vector>
#include <memory>

class Scene
{
public:
    std::vector<std::unique_ptr<GameObject>> objects;
    GameObject *selectedObject = nullptr;
    CameraObject *cameraObj = nullptr;

    void AddObject(std::unique_ptr<GameObject> obj)
    {
        objects.push_back(std::move(obj));
    }

    void Update(float deltaTime)
    {
        for (auto &obj : objects)
            obj->Update(deltaTime);
    }

    void Draw()
    {
        for (auto &obj : objects)
            obj->Draw();
    }

    void DrawUI()
    {
        ImGui::Begin("Scene Objects");
        for (auto &obj : objects)
        {
            if (ImGui::Selectable(obj->name.c_str(), obj.get() == selectedObject))
            {
                selectedObject = obj.get();
            }
        }
        ImGui::End();

        if (selectedObject)
        {
            ImGui::Begin("Inspector");
            selectedObject->DrawUI();
            ImGui::End();
        }
    }

    Camera3D GetActiveCamera()
    {
        if (cameraObj)
            return cameraObj->GetCamera();
        return {};
    }
};
