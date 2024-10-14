/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved
 * sean.writes.code@gmail.com
 */

#pragma once

#include <map>
#include <string>

struct SDL_Texture;

class AssetStore
{
public:
	AssetStore() = default;
	~AssetStore();

	void ClearAssets();
	void SetTexturePath(const std::string& NewPath);
	const std::string GetTexturePath() const { return TexturePath; }
	void AddTexture(const std::string& TextureID, const std::string& FileName);
	SDL_Texture* GetTexture(const std::string& TextureID);

private:
	std::map<std::string, SDL_Texture*> Textures;
	std::string TexturePath;
};