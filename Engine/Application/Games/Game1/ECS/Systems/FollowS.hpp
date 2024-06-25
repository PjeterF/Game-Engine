#pragma once

#include "../../src/ECS2/Systems/SysBase.hpp"

class FollowS : public SysBase
{
public:
	FollowS(int targetID=-1);
	virtual void update(float dt) override;
	bool setTarget(int ID);

	float acceleration = 0.0f;
private:
	int targetID = -1;
};