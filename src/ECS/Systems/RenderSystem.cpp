/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved
 * sean.writes.code@gmail.com
 */

#include "RenderSystem.h"
#include "ECS/Components/SpriteComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/BoxColliderComponent.h"
#include <SDL.h>
#include "Game/Game.h"
#include "Asset/AssetStore.h"

RenderSystem::RenderSystem()
{
    RequireComponent<SpriteComponent>();
    RequireComponent<TransformComponent>();
}

void RenderSystem::Update(const float DeltaTime)
{
    SDL_Renderer* renderer = Game::GetRenderer();
    AssetStore* assetManager = Game::GetAssetManager();

    if (renderer != nullptr && assetManager != nullptr)
    {
        SDL_RenderClear(renderer);

        for (const Entity& entity : GetEntities())
        {
            const auto& transform = entity.GetComponent<TransformComponent>();
            const auto& sprite = entity.GetComponent<SpriteComponent>();

            SDL_Texture* texture = assetManager->GetTexture(sprite.AssetID);
            SDL_Rect sourceRect = sprite.SourceRect;
            SDL_Rect destRect = {
                static_cast<int>(transform.Position.x),					// Origin X
                static_cast<int>(transform.Position.y),					// Origin Y
                static_cast<int>(sprite.Width * transform.Scale.x),		// Width
                static_cast<int>(sprite.Height * transform.Scale.x)		// Height
            };

            SDL_RenderCopyEx(renderer, texture, &sourceRect, &destRect, transform.Rotation, nullptr, SDL_FLIP_NONE);

            // Render debug collider shapes
            if (CoreStatics::IsDebugBuild && 
                CoreStatics::DrawDebugColliders && 
                entity.HasComponent<BoxColliderComponent>())
            {
                const auto& boxCollider = entity.GetComponent<BoxColliderComponent>();
                SDL_Rect colliderRect = {
                    static_cast<int>(transform.Position.x + boxCollider.Offset.x),
                    static_cast<int>(transform.Position.y + boxCollider.Offset.y),
                    static_cast<int>(boxCollider.Width),
                    static_cast<int>(boxCollider.Height)
                };

                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &colliderRect);
            }
        }

        SDL_RenderPresent(renderer);		
    }
    else
    {
        Logger::LogFatal("RenderSystem failed to get the renderer!");
    }

}

void RenderSystem::AddEntity(const Entity InEntity)
{
    const auto zOrder = InEntity.GetComponent<SpriteComponent>().ZOrder;

    if (EntityIDs.count(InEntity.GetID()) == 0)
    {
        EntityIDs.insert(InEntity.GetID());

        if (Entities.size() > 0)
        {
            auto it = Entities.begin();

            while (it != Entities.end())
            {
                if (it->GetComponent<SpriteComponent>().ZOrder >= zOrder)
                {
                    // Found the first zOrder greater or equal, insert immediately before it
                    break;
                }
                ++it;
            }

            Entities.insert(it, InEntity);
        }
        else
        {
            Entities.push_back(InEntity);
        }
    }
}
