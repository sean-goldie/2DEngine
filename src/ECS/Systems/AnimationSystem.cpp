/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved.
 * Contact: sean.writes.code@gmail.com
 */

#include "AnimationSystem.h"
#include "ECS/Components/SpriteComponent.h"
#include "ECS/Components/AnimationComponent.h"
#include "Util/CoreStatics.h"

AnimationSystem::AnimationSystem()
{
    RequireComponent<SpriteComponent>();
    RequireComponent<AnimationComponent>();
}

void AnimationSystem::Update(const float DeltaTime)
{
    for (const Entity& entity : GetEntities())
    {
        auto& animation = entity.GetComponent<AnimationComponent>();

        if (animation.ShouldLoop == false && animation.CurrentFrame >= animation.NumFrames)
        {
            continue;
        }

        if (CoreStatics::Now() >= animation.NextFrameUpdateTime)
        {
            auto& sprite = entity.GetComponent<SpriteComponent>();

            // CurrentFrame - 1 because the origin is 0, 0 and frames are not zero-indexed
            sprite.SourceRect.x = sprite.Width * (animation.CurrentFrame - 1);

            if (animation.CurrentFrame < animation.NumFrames)
            {
                ++animation.CurrentFrame;
            }
            else
            {
                animation.CurrentFrame = 1;
            }

            animation.NextFrameUpdateTime += animation.SecondsPerFrame;
        }
    }
}
