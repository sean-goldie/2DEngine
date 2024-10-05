#pragma once

#include "ECS/ECS.h"

class SpriteComponent : public Component<SpriteComponent>
{
public:
	int Width;
	int Height;

	SpriteComponent(const int Width = 32, const int Height = 32) :
		Width(Width), Height(Height)
	{

	}
};