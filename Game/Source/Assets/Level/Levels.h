#pragma once
#include "Core.hpp"

extern Engine::Core::Scene Level_0;          // Global Scene object // Actual definition
extern Engine::Graphics::TextureAtlas Level_0_Atlas;
extern Engine::Graphics::Animation PlayerAnimationFront;
extern Engine::Graphics::Animation PlayerAnimationBack;
extern Engine::Graphics::Animation PlayerAnimationRight;
extern Engine::Graphics::Animation PlayerAnimationLeft;
void Init_Level_0();           // Setup function
