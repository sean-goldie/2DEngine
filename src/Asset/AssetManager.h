#pragma once

#include "Util/Typedef.h"
#include <map>
#include <string>

typedef struct SDL_Texture Texture;

class AssetManager
{
public:
	void ClearAssets();
	void AddTexture(const std::string& TextureID, const std::string& FilePath);
	Texture* GetTexture(const std::string& TextureID);

private:
	std::map<std::string, Texture*> Textures;
};