#include <SDL.h>
#include "Game.h"
#include "Util/CoreStatics.h"
#include "Logger/Logger.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/RigidBodyComponent.h"
#include "ECS/Systems/MovementSystem.h"

Game::Game()
{
	GameManager = std::make_unique<ECSManager>();
}

void Game::Play()
{
	Initialize();
	Run();
	Destroy();
}

void Game::Setup()
{
	GameManager->AddSystem<MovementSystem>();

	// test test test
	// this will be game code here eventually
	// so we'll want to do this from a subclass of Game

	auto e = GameManager->CreateEntity();
	e.AddComponent<TransformComponent>();
	e.AddComponent<RigidBodyComponent>(glm::vec2(100.0, 100.0f));

	Logger::LogMessage("Created entity with ID " + std::to_string(e.GetID()));

}

void Game::ProcessInput()
{
	SDL_Event sdlEvent;

	while (SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
			// TODO: Give games a way to trigger this event.
		case SDL_QUIT:
			IsRunning = false;
			break;
			// Debug use escape key to quit everything.
		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE && CoreStatics::IsDebugBuild)
			{
				IsRunning = false;
			}
			break;
		}
	}
}

void Game::Update(const float DeltaTime)
{
	GameManager->Update(DeltaTime);
}

void Game::Render()
{

}

void Game::Initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		Logger::LogFatal("SDL failed to initialize");
		return;
	}

	// My preference is to debug in Windowed. So I built that into the engine for now :)
	if (CoreStatics::IsDebugBuild)
	{
		DisplayParameters.WindowedMode = SDLParameters::EWindowedMode::Windowed;
	}

	if (DisplayParameters.WindowedMode == SDLParameters::EWindowedMode::FullscreenMaxRes)
	{
		// For maximum possible resolution settings, query the display to find its dimensions
		SDL_DisplayMode displayMode;
		SDL_GetCurrentDisplayMode(0, &displayMode); // Only getting 0th display for now

		DisplayParameters.WindowWidth = displayMode.w;
		DisplayParameters.WindowHeight = displayMode.h;
	}

	SDLWindow = SDL_CreateWindow(
		nullptr,						// Window title 
		SDL_WINDOWPOS_CENTERED,			// X location
		SDL_WINDOWPOS_CENTERED,			// Y location
		DisplayParameters.WindowWidth,	// Width
		DisplayParameters.WindowHeight, // Height
		0								// Flags
	);

	if (SDLWindow == nullptr)
	{
		Logger::LogFatal("Error creating SDL window");
		return;
	}

	SDLRenderer = SDL_CreateRenderer(
		SDLWindow,	// Window to render
		-1,			// Index of display/driver to use (-1 is default)
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
		// Renderer flags - try to use GPU and VSync if possible
	);

	if (SDLRenderer == nullptr)
	{
		Logger::LogFatal("Error creating SDL renderer");
		return;
	}

	if (DisplayParameters.WindowedMode == SDLParameters::EWindowedMode::FullscreenMaxRes ||
		DisplayParameters.WindowedMode == SDLParameters::EWindowedMode::FullscreenOtherRes)
	{
		SDL_SetWindowFullscreen(SDLWindow, SDL_WINDOW_FULLSCREEN);
	}

	IsRunning = true;
}

void Game::Run()
{
	Setup();

	while (IsRunning)
	{
		ProcessInput();

		// Get delta time in milliseconds
		const unsigned MillisecsCurrentFrame = SDL_GetTicks();		

		// Convert to seconds for ease of use (conceptually, things should happen "per second")
		Update((MillisecsCurrentFrame - MillisecsPreviousFrame) * CoreStatics::OneMillisec);
		
		// Cache current milliseconds per frame to calculate next delta time
		MillisecsPreviousFrame = MillisecsCurrentFrame;

		Render();
	}
}

void Game::Destroy()
{
	SDL_DestroyRenderer(SDLRenderer);
	SDL_DestroyWindow(SDLWindow);
	SDL_Quit();
}