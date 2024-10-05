#pragma once

#include "ECS/ECS.h"
#include "glm/glm.hpp"

class RigidBodyComponent : public Component<RigidBodyComponent>
{
public:
	RigidBodyComponent(Vector2 Velocity = Vector2(0.0f, 0.0f)) : Velocity(Velocity) {}

	Vector2 Velocity;
};