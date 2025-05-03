#pragma once
#include "GameObject.hpp"
#include "raylib.h"
#include "raymath.h"

enum class ObjectCameraType
{
    FirstPerson,
    ThirdPerson
};

class CameraObject : public GameObject
{
public:
    Camera3D camera;
    ObjectCameraType cameraType = ObjectCameraType::FirstPerson;

    CameraObject(Vector3 pos) : GameObject("Camera", pos)
    {
        camera.position = pos;
        localPosition = pos;
        camera.target = {0, 0, 0};
        camera.up = {0, 1, 0};
        camera.fovy = 60;
        camera.projection = CAMERA_PERSPECTIVE;
    }

    void Update(float deltaTime) override
    {
        Vector3 globalPos = GetGlobalPosition();
        Vector3 globalRot = GetGlobalRotation();
        camera.position = globalPos;
        camera.target.x = globalPos.x + globalRot.x * DEG2RAD;
        camera.target.y = globalPos.y + sinf(globalRot.x * DEG2RAD);
        camera.target.z = globalPos.z + sinf(globalRot.y * DEG2RAD);
        }

    void Draw() override {} // Cameras don’t need to draw anything

    Camera3D GetCamera() { return camera; }
};
