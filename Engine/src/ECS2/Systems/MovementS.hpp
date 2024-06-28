#pragma once

#include "SysBase.hpp"

#include "../Components/Transform.hpp"
#include "../Components/Velocity.hpp"

class MovementS : public SysBase
{
public:
	MovementS();
	virtual void update(float dt) override;

	float velocityDecayFactor=0.9;
};