/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved.
 * Contact: sean.writes.code@gmail.com
 */

#pragma once

#include "ECS/ECS.h"

class CollisionEvent;

class BoxCollisionSystem : public System 
{
public:
    BoxCollisionSystem();

    void Update(const float DeltaTime) override;

private:
    const bool DetectCollision(const class BoxColliderComponent& ABox, 
        const BoxColliderComponent& BBox,
        const class TransformComponent& ATransform, 
        const TransformComponent& BTransform);

    void HandleCollision(const Entity& A, const Entity& B);

    void TestCallback(CollisionEvent& Event);
};