#pragma once

#include "SysBase.hpp"

#include "../Components/Sprite.hpp"
#include "../Components/Animation.hpp"
#include "../Components/Velocity.hpp"

class AnimationS : public SysBase
{
public:
	AnimationS();
	virtual void update(float dt) override;
};