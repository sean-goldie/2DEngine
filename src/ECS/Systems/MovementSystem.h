#pragma once
#include "ECS/ECS.h"

class MovementSystem : public System
{
public:
	MovementSystem();
	~MovementSystem() = default;

	void Update() override;
};