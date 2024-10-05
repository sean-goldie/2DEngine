#include "RenderSystem.h"
#include "ECS/Components/SpriteComponent.h"
#include "ECS/Components/TransformComponent.h"
#include <SDL.h>
#include "Game/Game.h"

RenderSystem::RenderSystem()
{
	RequireComponent<SpriteComponent>();
	RequireComponent<TransformComponent>();
}

void RenderSystem::Update(const float DeltaTime)
{
	for (const Entity& entity : GetEntities())
	{
		const auto& transform = entity.GetComponent<TransformComponent>();
		const auto& sprite = entity.GetComponent<SpriteComponent>();

		SDL_Rect object = { (int) transform.Position.x, (int) transform.Position.y,
							sprite.Width, sprite.Height };

		if (SDL_Renderer* Renderer = Game::GetRenderer())
		{
			
			SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(Renderer, &object);
			SDL_RenderPresent(Renderer);
		}
	}
}
