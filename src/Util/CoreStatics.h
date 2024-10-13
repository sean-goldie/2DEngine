#pragma once

#include <SDL.h>

namespace CoreStatics
{
	constexpr static bool IsDebugBuild = _DEBUG;
	constexpr static float OneMillisec = 1.0f / 1000.0f;
	constexpr static unsigned int MaxNumComponentTypes = 32;
	constexpr static unsigned int MaxNumEntities = -1;

	static const double Now()
	{
		return SDL_GetTicks() * OneMillisec;
	}
} // ~namespace CoreStatics