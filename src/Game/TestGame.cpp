#include "TestGame.h"
#include "ECS/Components/AllComponents.h"
#include "ECS/Systems/AllSystems.h"
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

	// test test test

	Entity tank = GameManager->CreateEntity();
	tank.AddComponent<TransformComponent>(Vector2(10.0, 10.0), Vector2(1.0, 1.0), 0.0);
	tank.AddComponent<RigidBodyComponent>(Vector2(10.0, 0.0));
	tank.AddComponent<SpriteComponent>("tank-image");

	Entity truck = GameManager->CreateEntity();
	truck.AddComponent<TransformComponent>(Vector2(50.0, 100.0), Vector2(1.0, 1.0), 0.0);
	truck.AddComponent<RigidBodyComponent>(Vector2(0.0, 10.0));
	truck.AddComponent<SpriteComponent>("truck-image");
}
