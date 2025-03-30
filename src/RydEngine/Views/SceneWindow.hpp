#pragma once
#include "raylib.h"
#include "raymath.h"
#include "rlImGui.h"
#include "rlgl.h"
#include "imgui.h"

#include "DocumentWindow.hpp"
#include "../Helpers/DpiScaling.hpp"
#include "../Entities/BSP.hpp"

#include <filesystem>
#include <set>
#include <vector>

class SceneViewWindow : public DocumentWindow
{
protected:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;

private:
    float moveSpeed = 10.0;
    float mouseSensitivity = 0.05;

public:
    Camera3D Camera = {0};
    std::string currentFile = "/Assets/Maps/BSP/dm4.bsp";
    std::vector<Model> models;

    void Setup() override
    {
        ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

        Camera.fovy = 45;
        Camera.up.y = 1;
        Camera.position.y = 3;
        Camera.position.z = -25;
        Camera.projection = CAMERA_PERSPECTIVE;

        Image img = GenImageChecked(ScaleToDPII(256), ScaleToDPII(256), ScaleToDPII(32), ScaleToDPII(32), DARKGRAY, WHITE);
        GridTexture = LoadTextureFromImage(img);
        UnloadImage(img);
        GenTextureMipmaps(&GridTexture);
        SetTextureFilter(GridTexture, TEXTURE_FILTER_ANISOTROPIC_16X);
        SetTextureWrap(GridTexture, TEXTURE_WRAP_CLAMP);

        rlEnableBackfaceCulling();
    }

    void Shutdown() override
    {
        UnloadRenderTexture(ViewTexture);
        UnloadTexture(GridTexture);
    }

    void Show() override
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::SetNextWindowSizeConstraints(ImVec2(ScaleToDPIF(400.0f), ScaleToDPIF(400.0f)), ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));

        if (ImGui::Begin("3D View", &Open, ImGuiWindowFlags_NoScrollbar))
        {

            ImGui::Text("FPS: %i", GetFPS());
            ImGui::SameLine();

            if (models.empty())
            {
                ImGui::Text("Drag and drop a .bsp file here");
                ImGui::SameLine();
            }

            ImGui::Text("Right click to move camera");
            ImGui::SameLine();
            ImGui::Text("WASD to move");
            ImGui::SameLine();

            if (ImGui::Button("Unload"))
            {
                std::for_each(models.begin(), models.end(), UnloadModel);
                models.clear();
            }

            Focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
            rlImGuiImageRenderTextureFit(&ViewTexture, true);
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void UpdateCameraVectors()
    {
        float cosPitch = cosf(rotation.x * DEG2RAD);
        Vector3 direction = {
            cosf(rotation.y * DEG2RAD) * cosPitch,
            sinf(rotation.x * DEG2RAD),
            sinf(rotation.y * DEG2RAD) * cosPitch};

        Camera.position = position;
        Camera.target = Vector3Add(position, direction);
    }

    void Update() override
    {
        if (!Open)
            return;

        if (IsWindowResized())
        {
            UnloadRenderTexture(ViewTexture);
            ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        }

        BeginTextureMode(ViewTexture);
        ClearBackground(SKYBLUE);

        BeginMode3D(Camera);

        if (IsFileDropped())
        {
            FilePathList droppedFiles = LoadDroppedFiles();

            if (std::filesystem::path(droppedFiles.paths[0]).extension() != ".bsp")
            {
                UnloadDroppedFiles(droppedFiles);
                return;
            }
            std::set<decltype(Texture::id)> textures{};
            for (Model model : models)
            {
                Texture texture = model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture;
                textures.insert(texture.id);
            }
            std::for_each(std::begin(textures), std::end(textures), rlUnloadTexture);

            std::for_each(models.begin(), models.end(), UnloadModel);

            currentFile = droppedFiles.paths[0];
            models = LoadModelsFromBSPFile(currentFile);
            UnloadDroppedFiles(droppedFiles);
        }

        if (IsKeyPressed(KEY_R))
            Camera.up = {0.0, 1.0, 0.0};

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);
            Vector2 mouseDelta = GetMouseDelta();
            rotation.y += mouseDelta.x * mouseSensitivity;
            rotation.x -= mouseDelta.y * mouseSensitivity;

            if (rotation.x > 89.0f)
                rotation.x = 89.0f;
            if (rotation.x < -89.0f)
                rotation.x = -89.0f;

            float cosPitch = cosf(rotation.x * DEG2RAD);
            Vector3 forward = {
                cosf(rotation.y * DEG2RAD) * cosPitch,
                sinf(rotation.x * DEG2RAD),
                sinf(rotation.y * DEG2RAD) * cosPitch};
            Vector3 right = Vector3CrossProduct(forward, {0, 1, 0});
            Vector3 up = {0, 1, 0};

            Vector3 moveDir = {0, 0, 0};
            if (IsKeyDown(KEY_W))
                moveDir = Vector3Add(moveDir, forward);
            if (IsKeyDown(KEY_S))
                moveDir = Vector3Subtract(moveDir, forward);
            if (IsKeyDown(KEY_D))
                moveDir = Vector3Add(moveDir, right);
            if (IsKeyDown(KEY_A))
                moveDir = Vector3Subtract(moveDir, right);
            if (IsKeyDown(KEY_SPACE))
                moveDir.y += 1.0f;
            if (IsKeyDown(KEY_LEFT_SHIFT))
                moveDir.y -= 1.0f;

            // on scroll change speed
            moveSpeed += GetMouseWheelMove() * 10.0f;
            if (moveSpeed < 1.0f)
                moveSpeed = 1.0f;

            if (!Vector3Equals(moveDir, {0, 0, 0}))
            {
                moveDir = Vector3Normalize(moveDir);
                moveDir = Vector3Scale(moveDir, moveSpeed * GetFrameTime());
                position = Vector3Add(position, moveDir);
            }
            DisableCursor();
        }
        else
        {
            if (IsCursorHidden())
            {
                EnableCursor();
            }

            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        UpdateCameraVectors();

        static bool enable_wireframe = false;

        ClearBackground(GRAY);

        for (Model model : models)
        {
            DrawModel(model, {}, 1.f, WHITE);
            if (enable_wireframe)
            {
                model.materials[0].shader = {rlGetShaderIdDefault(), rlGetShaderLocsDefault()};
                DrawModelWires(model, {}, 1.f, BLACK);
            }
        }

        EndMode3D();
        EndTextureMode();
    }

    Texture2D GridTexture = {0};
};
