/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved.
 * Contact: sean.writes.code@gmail.com
 */

#pragma once

#include "ECS/ECS.h"
#include "glm/glm.hpp"

using Vector2 = glm::vec2;

class BoxColliderComponent : public Component<BoxColliderComponent>
{
public:
    BoxColliderComponent(const int Width = 0, const int Height = 0, 
        const Vector2 Offset = {0, 0}) : Width(Width), Height(Height), Offset(Offset) {}

    void AddCollision(const unsigned int Other)
    {
        CollidingEntities.insert(Other);
    }

    void RemoveCollision(const unsigned int Other)
    {
        CollidingEntities.erase(Other);
    }

    const bool IsCollidingWith(const unsigned int Other) const
    {
        return CollidingEntities.count(Other);
    }

    int Width;
    int Height;
    Vector2 Offset;

private:
    std::unordered_set<unsigned int> CollidingEntities;
};