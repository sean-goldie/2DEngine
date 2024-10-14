/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved
 * sean.writes.code@gmail.com
 */

#pragma once

#include <string>
#include <unordered_map>

struct SDL_Window;
struct SDL_Renderer;
class ECSManager;
class AssetStore;

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

    static ECSManager* GetGameManager() { return GameManager; }
    static AssetStore* GetAssetManager() { return AssetManager; }
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

    /** Load a new level using string ID TilemapTextureID and a map file at MapFilePath */
    void LoadLevel(const std::string& TilemapTextureID, const std::string& MapFilePath);

    /** Display parameters. Can be edited from game subclasses of this class. */
    SDLParameters DisplayParameters;

    static ECSManager* GameManager;
    static AssetStore* AssetManager;
    static SDL_Renderer* SDLRenderer;

private:
    void Initialize();
    void Run();
    void Destroy();

private:
    bool IsRunning = false;
    SDL_Window* SDLWindow = nullptr;
    unsigned int MillisecsPreviousFrame = 0;
};