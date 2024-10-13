#include "TestGame.h"
#include "ECS/Systems/MovementSystem.h"
#include "ECS/Systems/AnimationSystem.h"
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/BoxCollisionSystem.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/AnimationComponent.h"
#include "ECS/Components/RigidBodyComponent.h"
#include "ECS/Components/SpriteComponent.h"
#include "ECS/Components/BoxColliderComponent.h"
#include "Asset/AssetStore.h"

TestGame::TestGame()
{
	// Override fullscreen mode for debugging
	DisplayParameters.WindowedMode = SDLParameters::EWindowedMode::Windowed;
}

void TestGame::Setup()
{
	Game::Setup();

	GameManager->AddSystem<MovementSystem>();
	GameManager->AddSystem<RenderSystem>();
	GameManager->AddSystem<AnimationSystem>();
	GameManager->AddSystem<BoxCollisionSystem>();

	const std::string tilemapDir = "./assets/tilemaps/";

	AssetManager->SetTexturePath(tilemapDir);
	AssetManager->AddTexture("Tilemap", "jungle.png");

	Game::LoadLevel(
		"Tilemap",						// Texture ID
		tilemapDir + "jungle.map"		// Map file name
	);

	AssetManager->SetTexturePath("./assets/images/");
	AssetManager->AddTexture("tank-image", "tank-panther-right.png");
	AssetManager->AddTexture("truck-image", "truck-ford-right.png");
	AssetManager->AddTexture("chopper-image", "chopper.png");
	AssetManager->AddTexture("radar-image", "radar.png");

	// test test test

	Entity tank = GameManager->CreateEntity();
	tank.AddComponent<TransformComponent>(Vector2(1000.0, 500.0), Vector2(1.0, 1.0), 0.0);
	tank.AddComponent<RigidBodyComponent>(Vector2(-30.0, 0.0));
	tank.AddComponent<SpriteComponent>("tank-image");
	tank.AddComponent<BoxColliderComponent>(32, 32);

	Entity truck = GameManager->CreateEntity();
	truck.AddComponent<TransformComponent>(Vector2(500.0, 500.0), Vector2(1.0, 1.0), 0.0);
	truck.AddComponent<RigidBodyComponent>(Vector2(20.0, 0.0));
	truck.AddComponent<SpriteComponent>("truck-image");
	truck.AddComponent<BoxColliderComponent>(32, 32);

	Entity helicopter = GameManager->CreateEntity();
	helicopter.AddComponent<TransformComponent>(Vector2(75.0, 100.0), Vector2(1.0, 1.0), 0.0);
	helicopter.AddComponent<RigidBodyComponent>(Vector2(10.0, 10.0));
	helicopter.AddComponent<SpriteComponent>("chopper-image");
	helicopter.AddComponent<AnimationComponent>(2);

	Entity radar = GameManager->CreateEntity();
	radar.AddComponent<TransformComponent>(Vector2(500.0, 250.0), Vector2(2.0, 2.0), 0.0);
	radar.AddComponent<SpriteComponent>("radar-image", 64, 64);
	radar.AddComponent<AnimationComponent>(8);
}
