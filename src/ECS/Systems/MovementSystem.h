#pragma once

#include "ECS/ECS.h"

class MovementSystem : public System
{
public:
	MovementSystem();
	~MovementSystem() = default;

	void Update(const float DeltaTime) override;
	void AddVelocity(const float X, const float Y);
};