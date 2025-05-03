#pragma once
#include "imgui.h"

namespace ImGuiTheme
{
    void SetupImGuiStyle()
    {
        ImVec4 *colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4(0.19f, 0.22f, 0.16f, 0.94f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.12f);
        colors[ImGuiCol_Border] = ImVec4(0.48f, 0.65f, 0.34f, 0.50f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.30f, 0.21f, 0.88f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.24f, 0.15f, 0.94f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.24f, 0.15f, 0.94f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.25f, 0.34f, 0.20f, 0.94f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.40f, 0.54f, 0.31f, 0.94f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.40f, 0.54f, 0.31f, 0.94f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.51f, 0.69f, 0.41f, 0.94f);
        colors[ImGuiCol_Button] = ImVec4(0.18f, 0.24f, 0.15f, 0.94f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.54f, 0.31f, 0.94f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.49f, 0.68f, 0.38f, 0.94f);
        colors[ImGuiCol_Header] = ImVec4(0.40f, 0.54f, 0.31f, 0.84f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.40f, 0.54f, 0.31f, 0.77f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.40f, 0.54f, 0.31f, 1.00f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.40f, 0.54f, 0.31f, 0.77f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.54f, 0.31f, 1.00f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.40f, 0.54f, 0.31f, 0.70f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.54f, 0.31f, 0.94f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.27f, 0.32f, 0.24f, 0.94f);
        colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.24f, 0.15f, 0.94f);
        colors[ImGuiCol_TabSelected] = ImVec4(0.25f, 0.29f, 0.22f, 1.00f);
        colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.21f, 0.24f, 0.19f, 0.94f);
        colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.18f, 0.24f, 0.15f, 0.94f);
        colors[ImGuiCol_DockingPreview] = ImVec4(0.40f, 0.54f, 0.31f, 0.62f);
        colors[ImGuiCol_TextLink] = ImVec4(0.44f, 0.74f, 0.26f, 0.94f);
        colors[ImGuiCol_NavCursor] = ImVec4(0.40f, 0.54f, 0.31f, 1.00f);
    }
}