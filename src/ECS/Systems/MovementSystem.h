/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved.
 * Contact: sean.writes.code@gmail.com
 */

#pragma once

#include "ECS/ECS.h"

class MovementSystem : public System
{
public:
    MovementSystem();
    ~MovementSystem() = default;

    void Update(const float DeltaTime) override;
    void AddVelocity(const float X, const float Y);
};