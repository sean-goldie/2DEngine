#pragma once

#include "ECS/ECS.h" // System

class RenderSystem : public System 
{
public:
	RenderSystem();

	void Update(const float DeltaTime) override;
	void AddEntity(const Entity InEntity) override;
};