#pragma once

#include "../../src/ECS2/Components/CBase.hpp"

struct MarbleShooter : public CBase
{
	MarbleShooter(float shotSpeed = 0, int cooldown = 0)
		: shotSpeed(shotSpeed), cooldown(cooldown), currentShotID(-1), nextShotID(-1), counter(0) {}

	int cooldown;
	int counter;
	int currentShotID, nextShotID;
	float shotSpeed;

	virtual nlohmann::json serialize() override;
	virtual void deSerialize(nlohmann::json j) override;
};