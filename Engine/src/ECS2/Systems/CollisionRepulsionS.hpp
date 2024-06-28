#pragma once

#include "SysBase.hpp"

class CollisionRepulsionS : public SysBase
{
public:
	CollisionRepulsionS();
	virtual void update(float dt) override;

	float repulsionStrength = 0.5f;
};