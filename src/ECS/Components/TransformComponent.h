#pragma once

#include "ECS/ECS.h"
#include "glm/glm.hpp"

class TransformComponent : public Component<TransformComponent>
{
public:
	TransformComponent(glm::vec2 Position = {0, 0}, glm::vec2 Scale = {1, 1}, 
		double Rotation = 0.0) : Position(Position), Scale(Scale), Rotation(Rotation) {};

	glm::vec2 Position;
	glm::vec2 Scale;
	double Rotation;
};