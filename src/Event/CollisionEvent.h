/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved.
 * Contact: sean.writes.code@gmail.com
 */

#pragma once

#include "Event.h"
#include "ECS/ECS.h"

class CollisionEvent : public Event
{
public:
    CollisionEvent(const Entity& A, const Entity& B)
        : A(A), B(B) {}

    Entity A;
    Entity B;
};