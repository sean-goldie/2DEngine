/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved
 * sean.writes.code@gmail.com
 */

#pragma once

#include "ECS/ECS.h"
#include "glm/glm.hpp"

using Vector2 = glm::vec2;

class BoxColliderComponent : public Component<BoxColliderComponent>
{
public:
	int Width;
	int Height;
	Vector2 Offset;

	BoxColliderComponent(const int Width = 0, const int Height = 0, 
		const Vector2 Offset = {0, 0}) : Width(Width), Height(Height), Offset(Offset) {}
};