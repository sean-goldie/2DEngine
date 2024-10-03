#include "MovementSystem.h"
#include "ECS/Components/TransformComponent.h"

MovementSystem::MovementSystem()
{
	RequireComponent<TransformComponent>();
}

void MovementSystem::Update()
{

}
