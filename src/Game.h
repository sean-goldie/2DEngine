#pragma once
#include <string>

struct SDL_Window;
struct SDL_Renderer;

/**
 * Rendering settings. Fullscreen mode is enabled by default.
 * TODO: implement a windowed setting
 */
struct DisplayParameters
{
	enum class EWindowedMode {
		FullscreenMaxRes = 0,
		FullscreenOtherRes,
		Windowed,
	};

	EWindowedMode WindowedMode = EWindowedMode::Windowed;

	// If WindowedMode is FullscreenMaxRes, these will be overwritten with 
	// the maximum values returned by a query on the display.
	size_t WindowWidth = 1920;
	size_t WindowHeight = 1080;
};

class Game
{
public:

	Game() {}
	~Game() = default;

	void Initialize();
	void Run();
	void Destroy();
	void Setup();
	void ProcessInput();
	void Update();
	void Render();
	void SetAssetPath(std::string newAssetPath);
	const std::string GetAssetPath() const { return AssetPath; }

private:

	bool IsRunning = false;

	// SDL variables
	SDL_Window* SDLWindow = nullptr;
	SDL_Renderer* SDLRenderer = nullptr;
	DisplayParameters SDLParameters;

	// Asset folder
	std::string AssetPath = "./assets/";
};