#pragma once

#include "ECS/ECS.h"

class RenderSystem : public System 
{
public:
	RenderSystem();

	void Update(const float DeltaTime) override;
};