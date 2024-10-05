#include "TestGame.h"
#include "ECS/Components/AllComponents.h"
#include "ECS/Systems/AllSystems.h"

void TestGame::Setup()
{
	Game::Setup();

	GameManager->AddSystem<MovementSystem>();
	GameManager->AddSystem<RenderSystem>();

	// test test test

	Entity tank = GameManager->CreateEntity();
	tank.AddComponent<TransformComponent>();
	tank.AddComponent<RigidBodyComponent>(glm::vec2(100.0, 100.0f));
	tank.AddComponent<SpriteComponent>();
}