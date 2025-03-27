#pragma once
#include "imgui.h"
#include "rlImGui.h"

class ImGuiManager
{
public:
    ImGuiManager()
    {
        rlImGuiSetup(true); // Use docking
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