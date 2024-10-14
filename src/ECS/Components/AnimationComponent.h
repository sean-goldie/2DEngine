/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved
 * sean.writes.code@gmail.com
 */

#pragma once

#include "ECS/ECS.h"

class AnimationComponent : public Component<AnimationComponent>
{
public:
	AnimationComponent(const size_t NumFrames = 1, const size_t CurrentFrame = 1,
		const int FrameRate = 5, const bool ShouldLoop = true) :
		NumFrames(NumFrames), CurrentFrame(CurrentFrame), 
		FrameRate(FrameRate), ShouldLoop(ShouldLoop), 
		SecondsPerFrame(1.0f / FrameRate), NextFrameUpdateTime(SecondsPerFrame) {}

	size_t NumFrames;
	size_t CurrentFrame;
	int FrameRate;
	bool ShouldLoop;
	float SecondsPerFrame;
	float NextFrameUpdateTime;
};