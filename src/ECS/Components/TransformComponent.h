#pragma once

#include "ECS/ECS.h"
#include "glm/glm.hpp"

class TransformComponent : public Component<TransformComponent>
{
public:
	TransformComponent(Vector2 Position = {0, 0}, Vector2 Scale = {1, 1}, 
		float Rotation = 0.0f) : Position(Position), Scale(Scale), Rotation(Rotation)
	{

	}

	Vector2 Position;
	Vector2 Scale;
	float Rotation;
};