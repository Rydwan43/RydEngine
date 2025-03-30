#pragma once
#include "imgui.h"
#include "Entities/GameObject.hpp"
#include <vector>
#include <memory>

enum class ShapeType
{
    Cube,
    Sphere
};

struct ShapeParams
{
    float cubeSize = 1.0f;
    float sphereRadius = 1.0f;
    int sphereSegments = 16;
    Color shapeColor = RED;
    ShapeType currentShape = ShapeType::Cube;
};

class UIManager
{
private:
    ShapeParams &params;
    std::vector<std::unique_ptr<GameObject>> &entities;

public:
    UIManager(ShapeParams &shapeParams, std::vector<std::unique_ptr<GameObject>> &entityList)
        : params(shapeParams), entities(entityList) {}

    void Draw()
    {
        ImGui::Begin("Tools");

        // Shape selection
        const char *shapes[] = {"Cube", "Sphere"};
        int shapeIndex = static_cast<int>(params.currentShape);
        if (ImGui::Combo("Shape", &shapeIndex, shapes, IM_ARRAYSIZE(shapes)))
        {
            params.currentShape = static_cast<ShapeType>(shapeIndex);
        }

        // Shape-specific controls
        if (params.currentShape == ShapeType::Cube)
        {
            ImGui::SliderFloat("Cube Size", &params.cubeSize, 0.1f, 5.0f);
        }
        else if (params.currentShape == ShapeType::Sphere)
        {
            ImGui::SliderFloat("Sphere Radius", &params.sphereRadius, 0.1f, 5.0f);
            ImGui::SliderInt("Sphere Segments", &params.sphereSegments, 3, 32);
        }

        // Color picker for all shapes
        float color[4] = {
            params.shapeColor.r / 255.0f,
            params.shapeColor.g / 255.0f,
            params.shapeColor.b / 255.0f,
            params.shapeColor.a / 255.0f};

        if (ImGui::ColorEdit4("Shape Color", color))
        {
            params.shapeColor = Color{
                (unsigned char)(color[0] * 255),
                (unsigned char)(color[1] * 255),
                (unsigned char)(color[2] * 255),
                (unsigned char)(color[3] * 255)};
        }

        ImGui::Text("Entities: %zu", entities.size());
        if (ImGui::Button("Clear All"))
        {
            entities.clear();
        }

        ImGui::End();
    }
};