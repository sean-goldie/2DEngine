#include "TestGame.h"
#include "ECS/Components/AllComponents.h"
#include "ECS/Systems/AllSystems.h"

typedef glm::vec2 Vector2;

void TestGame::Setup()
{
	Game::Setup();

	GameManager->AddSystem<MovementSystem>();
	GameManager->AddSystem<RenderSystem>();

	// test test test

	Entity tank = GameManager->CreateEntity();
	tank.AddComponent<TransformComponent>(
		Vector2(DisplayParameters.WindowWidth / 2, 100), // Position
		Vector2(1, 1), // Scale
		0.0f // Rotation
	);
	tank.AddComponent<RigidBodyComponent>(Vector2(100.0f, 0.0f));
	tank.AddComponent<SpriteComponent>(100, 10);
}