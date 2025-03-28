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
    }

    ~ImGuiManager()
    {
        rlImGuiShutdown();
    }

    void BeginFrame()
    {
        rlImGuiBegin();
    }

    void EndFrame()
    {
        rlImGuiEnd();
    }
};