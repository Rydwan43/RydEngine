#pragma once
#include "imgui.h"
#include "Settings.hpp"
#include <functional>

class MenuManager
{
private:
    bool showMenu = false;
    bool showOptions = false;
    Settings &settings;
    std::function<void()> quitCallback;

    void DrawMainMenu()
    {
        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(viewport->GetCenter().x, viewport->GetCenter().y), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_Always);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
        if (ImGui::Begin("Main Menu", nullptr,
                         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 10));

            const float buttonWidth = 200.0f;
            const float buttonHeight = 40.0f;

            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) * 0.5f);
            if (ImGui::Button("Load Map", ImVec2(buttonWidth, buttonHeight)))
            {
                // TODO: Implement map loading
            }

            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) * 0.5f);
            ImGui::BeginDisabled(true); // Disabled for now
            if (ImGui::Button("Save Map", ImVec2(buttonWidth, buttonHeight)))
            {
                // TODO: Implement map saving
            }
            ImGui::EndDisabled();

            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) * 0.5f);
            if (ImGui::Button("Options", ImVec2(buttonWidth, buttonHeight)))
            {
                showOptions = true;
            }

            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) * 0.5f);
            if (ImGui::Button("Quit", ImVec2(buttonWidth, buttonHeight)))
            {
                if (quitCallback)
                    quitCallback();
            }

            ImGui::PopStyleVar(); // ItemSpacing
        }
        ImGui::End();
        ImGui::PopStyleVar(); // WindowPadding
    }

    void DrawOptions()
    {
        ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Options", &showOptions))
        {
            if (ImGui::CollapsingHeader("Display", ImGuiTreeNodeFlags_DefaultOpen))
            {
                bool fullscreen = settings.fullscreen;
                if (ImGui::Checkbox("Fullscreen", &fullscreen))
                {
                    settings.fullscreen = fullscreen;
                    settings.ApplySettings();
                }

                int resX = settings.resolutionX;
                int resY = settings.resolutionY;
                if (ImGui::InputInt("Width", &resX) || ImGui::InputInt("Height", &resY))
                {
                    if (resX > 0 && resY > 0)
                    {
                        settings.resolutionX = resX;
                        settings.resolutionY = resY;
                        settings.ApplySettings();
                    }
                }

                bool vSync = settings.vSync;
                if (ImGui::Checkbox("VSync", &vSync))
                {
                    settings.vSync = vSync;
                    settings.ApplySettings();
                }
            }
        }
        ImGui::End();
    }

public:
    MenuManager(Settings &settings, std::function<void()> quitCallback)
        : settings(settings), quitCallback(quitCallback) {}

    void Update()
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            showMenu = !showMenu;
            if (showMenu)
            {
                EnableCursor();
            }
            else
            {
                DisableCursor();
                showOptions = false;
            }
        }

        if (showMenu)
        {
            DrawMainMenu();
            if (showOptions)
            {
                DrawOptions();
            }
        }
    }

    bool IsMenuVisible() const { return showMenu; }
};