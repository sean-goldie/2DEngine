#pragma once

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
	/** Go!! */
	void Play();

	/** Resolution and display mode parameters */
	SDLParameters DisplayParameters;

private:
	void Setup();
	void ProcessInput();
	void Update(const double DeltaTime);
	void Render();
	void Initialize();
	void Run();
	void Destroy();

	bool IsRunning = false;

	SDL_Window* SDLWindow = nullptr;
	SDL_Renderer* SDLRenderer = nullptr;

	//std::string AssetPath = "./assets/";

	// TODO Test BS
	//std::string TestAssetPath = AssetPath + "images/tank-tiger-right.png";
	//SDL_Texture* TestTexture = nullptr;

	unsigned int MillisecsPreviousFrame = 0;
};