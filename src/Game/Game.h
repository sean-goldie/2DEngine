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
 * Abstract game instance base class. All games must inherit from this class.
 * Keep in mind that if your game's class does not override Setup() with its
 * systems and game objects, you will have no content!
 */
class Game
{
public:
	Game();

	/** Go!! */
	void Play();

	static SDL_Renderer* GetRenderer() { return SDLRenderer; }

protected:
	/** Generic setup routine. Game-specific logic can be extended in game classes. */
	virtual void Setup();

	/** Generic input routine. Game-specific logic can be extended in game classes. */
	virtual void ProcessInput();

	/** Generic update loop. Game-specific logic can be extended in game classes. */
	virtual void Update(const float DeltaTime);

	/** Generic render loop. Game-specific logic can be extended in game classes. */
	virtual void Render(const float DeltaTime);

private:
	void Initialize();
	void Run();
	void Destroy();

protected:
	std::shared_ptr<ECSManager> GameManager = nullptr;
	SDLParameters DisplayParameters;

private:
	bool IsRunning = false;

	// Raw pointers returned from SDL C API
	SDL_Window* SDLWindow = nullptr;
	static SDL_Renderer* SDLRenderer;

	unsigned int MillisecsPreviousFrame = 0;
};