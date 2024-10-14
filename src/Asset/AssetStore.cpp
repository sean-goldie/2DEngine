/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved
 * sean.writes.code@gmail.com
 */

#include "AssetStore.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Game/Game.h"
#include <cassert>
#include "Logger/Logger.h"

AssetStore::~AssetStore()
{
    ClearAssets();
}

void AssetStore::ClearAssets()
{
    for (auto& [key, texture] : Textures)
    {
        SDL_DestroyTexture(texture);
    }
}

void AssetStore::SetTexturePath(const std::string& NewPath)
{
    TexturePath = NewPath;
    Logger::LogMessage("Changing texture path to " + NewPath);
}

void AssetStore::AddTexture(const std::string& TextureID, const std::string& FileName)
{
    assert(Textures.count(TextureID) == 0);

    std::string fullPath = TexturePath + FileName;

    if (SDL_Surface* surface = IMG_Load(fullPath.c_str()))
    {
        if (SDL_Renderer* renderer = Game::GetRenderer())
        {
            if (SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface))
            {
                Textures[TextureID] = texture;
                Logger::LogMessage("Added texture with ID " + TextureID);
            }
        }
        SDL_FreeSurface(surface);
    }
    else
    {
        Logger::LogError("Failed to load asset at path " + fullPath);
    }
}

SDL_Texture* AssetStore::GetTexture(const std::string& TextureID)
{
    if (Textures.count(TextureID))
    {
        return Textures[TextureID];
    }
    return nullptr;
}
