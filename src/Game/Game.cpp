/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved.
 * Contact: sean.writes.code@gmail.com
 */

#include <SDL.h>
#include <fstream>
#include <ostream>
#include "Game.h"
#include "Asset/AssetStore.h"
#include "Util/CoreStatics.h"
#include "EventBus/EventBus.h"
#include "ECS/Systems/RenderSystem.h" // includes ECS.h
#include "glm/glm.hpp"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/SpriteComponent.h"

ECSManager* Game::GameManager = nullptr;
AssetStore* Game::AssetManager = nullptr;
SDL_Renderer* Game::SDLRenderer = nullptr;
EventBus* Game::EventManager = nullptr;

Game::Game()
{
    GameManager = new ECSManager;
    AssetManager = new AssetStore;
    EventManager = new EventBus;
}

void Game::Play()
{
    Initialize();
    Run();
    Destroy();
}

void Game::Setup()
{

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
        case SDL_KEYDOWN:
            // Debug-only keybinds
            if (CoreStatics::IsDebugBuild)
            {
                // Debug use escape key to quit everything.
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
                {
                    IsRunning = false;
                }
                // Debug use F1 to toggle collider rendering
                else if (sdlEvent.key.keysym.sym == SDLK_F1)
                {
                    CoreStatics::DrawDebugColliders = !CoreStatics::DrawDebugColliders;
                }
            }
            break;
        }
    }
}

void Game::Update(const float DeltaTime)
{
    GameManager->Update(DeltaTime);
}

void Game::Render(const float DeltaTime)
{

}

void Game::LoadLevel(const std::string& TilemapTextureID, const std::string& MapFilePath)
{
    const SDL_Texture* tilemapTexture = AssetManager->GetTexture(TilemapTextureID);

    if (tilemapTexture == nullptr)
    {
        Logger::LogError("Couldn't retrieve tilemap texture with ID " + TilemapTextureID);
    }

    // Open the map file
    std::fstream mapFile(MapFilePath);

    if (mapFile.is_open() == false)
    {
        Logger::LogError("Couldn't open map file at location " + MapFilePath);
    }

    std::vector<std::vector<std::string>> tileValues;
    int mapNumRows = 0;
    int mapNumCols = 0;

    std::string currentLine;
    while (std::getline(mapFile, currentLine))
    {
        tileValues.push_back(std::vector<std::string>());

        int currentCol = 0;
        std::string currentVal;

        for (const char c : currentLine)
        {
            if (c != ',')
            {
                currentVal += c;
            }
            else
            {
                tileValues[mapNumRows].push_back(currentVal);
                currentVal = "";
                ++currentCol;
            }
        }

        if (mapNumCols == 0)
        {
            mapNumCols = currentCol;
        }

        ++mapNumRows;
    }

    const int tileSize = 32;
    const double tileScale = (
        static_cast<double>(DisplayParameters.WindowWidth) / 
        static_cast<double>(mapNumRows * tileSize)
    );

    int y = 0;
    for (const std::vector<std::string>& col : tileValues)
    {
        int x = 0;
        for (const std::string& row : col)
        {
            // Should always pass - first char is encoded column number, second is row
            if (row.length() == 2)
            {
                int currentSrcRectY = (row[0] - '0') * tileSize;
                int currentSrcRectX = (row[1] - '0') * tileSize;

                Entity tile = GameManager->CreateEntity();

                tile.AddComponent<TransformComponent>(
                    Vector2(
                        x++ * tileScale * tileSize, 
                        y * tileScale * tileSize
                    ),
                    Vector2(tileScale, tileScale)
                );

                tile.AddComponent<SpriteComponent>(
                    "Tilemap",
                    tileSize, tileSize,
                    currentSrcRectX,
                    currentSrcRectY,
                    -1
                );
            }
        }
        ++y;
    }

    mapFile.close();

}

void Game::Initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        Logger::LogFatal("SDL failed to initialize");
        return;
    }

    if (DisplayParameters.WindowedMode == SDLParameters::EWindowedMode::FullscreenMaxRes)
    {
        // For maximum possible resolution settings, query the display to find its dimensions
        SDL_DisplayMode displayMode;
        SDL_GetCurrentDisplayMode(0, &displayMode);

        DisplayParameters.WindowWidth = displayMode.w;
        DisplayParameters.WindowHeight = displayMode.h;
    }

    SDLWindow = SDL_CreateWindow(
        nullptr,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        DisplayParameters.WindowWidth,
        DisplayParameters.WindowHeight,
        0
    );

    if (SDLWindow == nullptr)
    {
        Logger::LogFatal("Error creating SDL window");
        return;
    }

    SDLRenderer = SDL_CreateRenderer(
        SDLWindow,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
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
        const float deltaTime = (MillisecsCurrentFrame - MillisecsPreviousFrame) * CoreStatics::OneMillisec;

        Update(deltaTime);
        
        // Cache current milliseconds per frame to calculate next delta time
        MillisecsPreviousFrame = MillisecsCurrentFrame;

        Render(deltaTime);
    }
}

void Game::Destroy()
{
    SDL_DestroyRenderer(SDLRenderer);
    SDL_DestroyWindow(SDLWindow);
    SDL_Quit();

    delete GameManager;
    delete AssetManager;
    delete EventManager;
}