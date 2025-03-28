#pragma once

enum class EngineMode
{
    Edit,
    Play
};

struct EngineState
{
    EngineMode mode = EngineMode::Edit;
    bool justSwitched = false;
};