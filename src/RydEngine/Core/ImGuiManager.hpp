#pragma once
#include "imgui.h"
#include "rlImGui.h"
#include "ImGuiTheme.hpp"

class ImGuiManager
{
public:
    ImGuiManager()
    {
        rlImGuiSetup(true); // Use docking
        ImGuiTheme::SetupImGuiStyle();
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable gamepad controls
    }

    ~ImGuiManager()
    {
        rlImGuiShutdown();
    }

    void BeginFrame()
    {
        rlImGuiBegin();
        ImGui::ShowDemoWindow();
    }

    void EndFrame()
    {
        rlImGuiEnd();
    }
};