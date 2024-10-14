/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved
 * sean.writes.code@gmail.com
 */

#pragma once

#include "ECS/ECS.h" // System

class RenderSystem : public System 
{
public:
    RenderSystem();

    void Update(const float DeltaTime) override;
    void AddEntity(const Entity InEntity) override;
};