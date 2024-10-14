/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved.
 * Contact: sean.writes.code@gmail.com
 */

#pragma once

#include "ECS/ECS.h"
#include "glm/glm.hpp"

class RigidBodyComponent : public Component<RigidBodyComponent>
{
public:
    RigidBodyComponent(Vector2 Velocity = Vector2(0.0, 0.0)) : Velocity(Velocity) {}

    Vector2 Velocity;
};