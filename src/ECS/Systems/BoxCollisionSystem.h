/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved
 * sean.writes.code@gmail.com
 */

#pragma once

#include "ECS/ECS.h"

class BoxCollisionSystem : public System 
{
public:
	BoxCollisionSystem();

	void Update(const float DeltaTime) override;

private:
	const bool DetectCollision(const Entity& A, const Entity& B);
};