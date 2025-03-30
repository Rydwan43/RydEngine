#pragma once
#include "raylib.h"

// create a header file for DPI scaling
// float ScaleToDPIF(float value)
// {
//     return GetWindowScaleDPI().x * value;
// }

// int ScaleToDPII(int value)
// {
//     return int(GetWindowScaleDPI().x * value);
// }

float ScaleToDPIF(float value)
{
    return GetWindowScaleDPI().x * value;
}

int ScaleToDPII(int value)
{
    return int(GetWindowScaleDPI().x * value);
}