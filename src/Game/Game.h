#pragma once

#include <memory>
#include "ECS/ECS.h" // Must be included here to allocate unique_ptr<ECSManager>

struct SDL_Window;
struct SDL_Renderer;

/**
 * Rendering settings. Fullscreen mode is enabled by default.
 */
struct SDLParameters
{
	enum class EWindowedMode 
	{
		FullscreenMaxRes = 0,
		FullscreenOtherRes,
		Windowed,
	};

	EWindowedMode WindowedMode = EWindowedMode::FullscreenMaxRes;

	/** If WindowedMode is FullscreenMaxRes, these will be overwritten with
	 * the maximum values returned by a query on the display.
	 */
	int WindowWidth = 1920;
	int WindowHeight = 1080;
};

/**
 * Game instance singleton
 */
class Game
{
public:
	Game();

	/** Go!! */
	void Play();

	/** Resolution and display mode parameters */
	SDLParameters DisplayParameters;

private:
	void Setup();
	void ProcessInput();
	void Update(const float DeltaTime);
	void Render();
	void Initialize();
	void Run();
	void Destroy();

	bool IsRunning = false;

	// Raw pointers returned from SDL C API
	SDL_Window* SDLWindow = nullptr;
	SDL_Renderer* SDLRenderer = nullptr;

	std::shared_ptr<ECSManager> GameManager = nullptr;

	unsigned int MillisecsPreviousFrame = 0;
};