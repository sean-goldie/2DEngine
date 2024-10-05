#include "MovementSystem.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/RigidBodyComponent.h"

MovementSystem::MovementSystem()
{
	RequireComponent<TransformComponent>();
	RequireComponent<RigidBodyComponent>();
}

void MovementSystem::Update(const float DeltaTime)
{
	for (const Entity& entity : GetEntities())
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		const auto& rigidBody = entity.GetComponent<RigidBodyComponent>();

		transform.Position.x += rigidBody.Velocity.x * DeltaTime;
		transform.Position.y += rigidBody.Velocity.y * DeltaTime;

		Logger::LogMessage("Entity " + std::to_string(entity.GetID()) + 
							" position is now " + std::to_string(transform.Position.x) +
							',' + std::to_string(transform.Position.y));
	}
}

void MovementSystem::AddVelocity(const float X, const float Y)
{
	
}
