#pragma once

#include "../../src/ECS2/Systems/SysBase.hpp"

class FollowS : public SysBase
{
public:
	FollowS(int targetID=-1);
	virtual void handleEvent(Event& event) override;
	virtual void update(float dt) override;
	bool setTarget(int ID);

	float acceleration = 0.0f;
	float interceptStrength = 0.0f;
	float rePositioningDistance = 1500;
	float teleportDistance = 1000;
private:
	int targetID = -1;
};