#pragma once
#include <string>
#include "raylib.h"

class Settings
{
public:
    int resolutionX;
    int resolutionY;
    std::string title;
    int targetFPS;
    bool vSync;
    bool fullscreen;
    bool borderless;
    bool resizeable;
    bool showFPS;

    Settings() : resolutionX(1024),
                 resolutionY(798),
                 title("RydEngine"),
                 targetFPS(60),
                 vSync(false),
                 fullscreen(false),
                 borderless(false),
                 resizeable(true),
                 showFPS(false) {}

    void ApplySettings()
    {
        SetWindowSize(resolutionX, resolutionY);
        SetWindowTitle(title.c_str());
        SetTargetFPS(targetFPS);
        SetConfigFlags(vSync ? FLAG_VSYNC_HINT : 0);
        if (fullscreen)
            ToggleFullscreen();
        SetConfigFlags(borderless ? FLAG_WINDOW_UNDECORATED : 0);
        SetConfigFlags(resizeable ? FLAG_WINDOW_RESIZABLE : 0);
    }
};