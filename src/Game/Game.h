#pragma once

#include <string>

struct SDL_Window;
struct SDL_Renderer;

// TODO need this now?? Just for test code - for now
struct SDL_Texture;

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
	Game() {}
	~Game() = default;

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

	/** Core */
	bool IsRunning = false;

	/** SDL variables */
	SDL_Window* SDLWindow = nullptr;
	SDL_Renderer* SDLRenderer = nullptr;

	/** Asset variables */
	std::string AssetPath = "./assets/";

	// TODO Test BS
	std::string TestAssetPath = AssetPath + "images/tank-tiger-right.png";
	SDL_Texture* TestTexture = nullptr;

	/** Time */
	unsigned MillisecsPreviousFrame = 0;
};