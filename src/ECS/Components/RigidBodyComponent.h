#pragma once

#include "ECS/ECS.h"
#include "glm/glm.hpp"

class RigidBodyComponent : public Component<RigidBodyComponent>
{
public:
	RigidBodyComponent(glm::vec2 Velocity = glm::vec2(0.0f, 0.0f)) : Velocity(Velocity) {}

	glm::vec2 Velocity;
};