#include "Game.h"
#include <SDL.h>
#include <iostream>

void Game::Initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cerr << "Error initializing SDL" << std::endl;
		return;
	}

	if (SDLParameters.WindowedMode == DisplayParameters::EWindowedMode::Fullscreen)
	{
		SDL_DisplayMode displayMode;
		SDL_GetCurrentDisplayMode(0, &displayMode); // Only getting 0th display for now

		SDLParameters.WindowWidth = displayMode.w;
		SDLParameters.WindowHeight = displayMode.h;
	}

	SDLWindow = SDL_CreateWindow(
		nullptr,					// Window title 
		SDL_WINDOWPOS_CENTERED,		// X location
		SDL_WINDOWPOS_CENTERED,		// Y location
		SDLParameters.WindowWidth,	// Width
		SDLParameters.WindowHeight, // Height
		0							// Flags
	);

	if (SDLWindow == nullptr)
	{
		std::cerr << "Error creating SDL window" << std::endl;
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
		std::cerr << "Error creating SDL renderer" << std::endl;
		return;
	}

	if (SDLParameters.WindowedMode == DisplayParameters::EWindowedMode::Fullscreen)
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
		Update();
		Render();
	}
}

void Game::Destroy()
{
	SDL_DestroyRenderer(SDLRenderer);
	SDL_DestroyWindow(SDLWindow);
	SDL_Quit();
}

void Game::Setup()
{
	// TODO setup game objects
}

void Game::ProcessInput()
{
	SDL_Event sdlEvent;
	
	while (SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
			case SDL_QUIT:
				IsRunning = false;
				break;
			// TODO: Temp/debug use escape key to quit everything. Need to implement SDL_QUIT instead
			case SDL_KEYDOWN:
				if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
				{
					IsRunning = false;
				}
				break;
		}
	}
}

void Game::Update()
{
	// TODO update game objects
}

void Game::Render()
{
	SDL_SetRenderDrawColor(SDLRenderer, 20, 20, 20, 255);
	SDL_RenderClear(SDLRenderer);

	SDL_RenderPresent(SDLRenderer);
}

