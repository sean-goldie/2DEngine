/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved
 * sean.writes.code@gmail.com
 */

#pragma once

#include "ECS/ECS.h"
#include <SDL.h>

class SpriteComponent : public Component<SpriteComponent>
{
public:
	/** String ID of the sprite texture to use. */
	std::string AssetID;

	/** Width of the sprite in pixels (default 32). */
	int Width;

	/** Height of the sprite in pixels (default 32). */
	int Height;

	/**
	 * Draw order. Lower indices will be drawn earlier (i.e. painted in the back)
	 * -1 is reserved for the tilemap, you should start your game objects at index 0.
	 * Default is 0.
	 */
	int ZOrder;

	/**
	 * The rectangle on the source texture from which to pull this asset's sprite.
	 * Measured in pixels from the top left corner of the texture (0,0) down to (Width,Height) 
	 * pixels. Default is (0,0).
	 */
	SDL_Rect SourceRect;

	SpriteComponent(const std::string& AssetID = "", 
		const int Width = 32, const int Height = 32,
		const int SourceRectX = 0, const int SourceRectY = 0, const int ZOrder = 0) :
			AssetID(AssetID), Width(Width), Height(Height), ZOrder(ZOrder)
	{
		SourceRect = { SourceRectX, SourceRectY, Width, Height };
	}
};