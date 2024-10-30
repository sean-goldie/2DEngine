/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved.
 * Contact: sean.writes.code@gmail.com
 */

#pragma once

#include "ECS/ECS.h"

class DamageSystem : public System
{
public:
    DamageSystem();

    void Update(const float DeltaTime) override;
    void TestCallback(class CollisionEvent& Event);
};