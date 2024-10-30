/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved.
 * Contact: sean.writes.code@gmail.com
 */

#include "BoxCollisionSystem.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/BoxColliderComponent.h"
#include "ECS/Components/RigidBodyComponent.h"
#include "Game/Game.h"
#include "EventBus/EventBus.h"
#include "Event/CollisionEvent.h"

BoxCollisionSystem::BoxCollisionSystem()
{
    RequireComponent<TransformComponent>();
    RequireComponent<BoxColliderComponent>();
}

void BoxCollisionSystem::Update(const float DeltaTime)
{
    const auto& entities = GetEntities();

    for (size_t i = 0; i < entities.size(); i++)
    {
        for (size_t j = i + 1; j < entities.size(); j++)
        {
            const Entity& a = entities.at(i);
            const Entity& b = entities.at(j);
            
            const unsigned int aID = a.GetID();
            const unsigned int bID = b.GetID();

            auto& aBox = a.GetComponent<BoxColliderComponent>();
            auto& bBox = b.GetComponent<BoxColliderComponent>();

            const auto& aTransform = a.GetComponent<TransformComponent>();
            const auto& bTransform = b.GetComponent<TransformComponent>();

            const bool alreadyColliding = aBox.IsCollidingWith(bID) && bBox.IsCollidingWith(aID);

            // Are objects overlapping?
            if (DetectCollision(aBox, bBox, aTransform, bTransform))
            {
                // Have we handled this collision yet?
                if (alreadyColliding == false)
                {
                    aBox.AddCollision(bID);
                    bBox.AddCollision(aID);
                    HandleCollision(a, b);
                }
            }
            // Collision handled in previous frame, objects no longer overlapping
            else if (alreadyColliding)
            {
                aBox.RemoveCollision(bID);
                bBox.RemoveCollision(aID);
            }
        }
    }
}

const bool BoxCollisionSystem::DetectCollision(
    const BoxColliderComponent& ABox, const BoxColliderComponent& BBox,
    const TransformComponent& ATransform, const TransformComponent& BTransform)
{
    const auto aMinX = ATransform.Position.x + ABox.Offset.x;
    const auto aMaxX = aMinX + ABox.Width;
    const auto bMinX = BTransform.Position.x + ABox.Offset.x;
    const auto bMaxX = bMinX + BBox.Width;
    const auto aMinY = ATransform.Position.y + ABox.Offset.y;
    const auto aMaxY = aMinY + ABox.Width;
    const auto bMinY = BTransform.Position.y + ABox.Offset.y;
    const auto bMaxY = bMinY + BBox.Width;

    return (
        aMinX < bMaxX &&
        bMinX < aMaxX &&
        aMinY < bMaxY &&
        bMinY < aMaxY
    );
}

void BoxCollisionSystem::HandleCollision(const Entity& A, const Entity& B)
{
    if (auto* eventManager = Game::GetEventManager())
    {
        eventManager->EmitEvent<CollisionEvent>(A, B);
    }
}