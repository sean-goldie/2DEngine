/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved
 * sean.writes.code@gmail.com
 */

#pragma once

#include "ECS/ECS.h"

class AnimationSystem : public System
{
public:
	AnimationSystem();
	void Update(const float DeltaTime) override;
};