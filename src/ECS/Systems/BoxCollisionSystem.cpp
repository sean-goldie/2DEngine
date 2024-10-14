/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved
 * sean.writes.code@gmail.com
 */

#include "BoxCollisionSystem.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/BoxColliderComponent.h"

BoxCollisionSystem::BoxCollisionSystem()
{
	RequireComponent<TransformComponent>();
	RequireComponent<BoxColliderComponent>();
}

void BoxCollisionSystem::Update(const float DeltaTime)
{
	const auto& entities = GetEntities();

	for (size_t i = 0; i < entities.size(); i++)
	{
		for (size_t j = i + 1; j < entities.size(); j++)
		{
			if (DetectCollision(entities.at(i), entities.at(j)))
			{
				Logger::LogWarning("Collision detected!");
			}
		}
	}
}

const bool BoxCollisionSystem::DetectCollision(const Entity& A, const Entity& B)
{
	const auto& aTransform = A.GetComponent<TransformComponent>();
	const auto& bTransform = B.GetComponent<TransformComponent>();
	auto& aBox = A.GetComponent<BoxColliderComponent>();
	auto& bBox = B.GetComponent<BoxColliderComponent>();

	return (
		aTransform.Position.x < bTransform.Position.x + bBox.Width &&
		bTransform.Position.x < aTransform.Position.x + aBox.Width &&
		aTransform.Position.y < bTransform.Position.y + bBox.Height &&
		bTransform.Position.y < aTransform.Position.y + aBox.Height
	);
}
