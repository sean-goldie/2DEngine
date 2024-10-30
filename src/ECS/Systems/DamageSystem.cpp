/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved.
 * Contact: sean.writes.code@gmail.com
 */

#include "DamageSystem.h"
#include "ECS/Components/BoxColliderComponent.h"
#include "Game/Game.h"
#include "Event/CollisionEvent.h"
#include "EventBus/EventBus.h"

DamageSystem::DamageSystem()
{
    RequireComponent<BoxColliderComponent>();

    if (auto* eventManager = Game::GetEventManager())
    {
        eventManager->RegisterHandler<DamageSystem, CollisionEvent>(this, &DamageSystem::TestCallback);
    }
}

void DamageSystem::Update(const float DeltaTime)
{
    if (auto* eventManager = Game::GetEventManager())
    {
        eventManager->UnRegisterHandler<DamageSystem, CollisionEvent>(this);
    }
}

void DamageSystem::TestCallback(CollisionEvent& Event)
{
    Logger::LogMessage("DamageSystem::TestCallback");
}
