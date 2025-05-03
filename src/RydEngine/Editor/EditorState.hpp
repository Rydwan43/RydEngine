#pragma once

enum class EditorMode
{
    Edit,
    Play
};

struct EditorState
{
    EditorMode mode = EditorMode::Edit;
    bool justSwitched = false;
};