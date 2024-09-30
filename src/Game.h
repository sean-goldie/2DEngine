#pragma once

struct SDL_Window;
struct SDL_Renderer;

/**
 * Rendering settings. Fullscreen mode is enabled by default.
 * TODO: implement a windowed setting
 */
struct DisplayParameters
{
	enum class EWindowedMode {
		Fullscreen = 0,
		Windowed,
		// TODO: others??
	};

	size_t WindowWidth = 800;
	size_t WindowHeight = 600;
	EWindowedMode WindowedMode = EWindowedMode::Fullscreen;
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

private:

	bool IsRunning = false;

	// SDL variables
	SDL_Window* SDLWindow = nullptr;
	SDL_Renderer* SDLRenderer = nullptr;
	DisplayParameters SDLParameters;
};