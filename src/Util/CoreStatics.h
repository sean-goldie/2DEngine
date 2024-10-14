/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved
 * sean.writes.code@gmail.com
 */

#pragma once

#include <SDL.h>

class CoreStatics
{
public:
    constexpr static bool IsDebugBuild = _DEBUG;
    static bool DrawDebugColliders;
    constexpr static float OneMillisec = 1.0f / 1000.0f;
    constexpr static unsigned int MaxNumComponentTypes = 32;
    constexpr static unsigned int MaxNumEntities = -1;

    static const double Now()
    {
        return SDL_GetTicks() * OneMillisec;
    }
};