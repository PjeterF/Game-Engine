#pragma once

#include "../../src/ECS/Components/ComponentBase.hpp"

class ShooterC : public ComponentBase
{
public:
	ShooterC(float shotSpeed = 0, int cooldown = 0);
	virtual void to_json(nlohmann::json& j) const;
	virtual void from_json(nlohmann::json& j);

	int currShotEntID = -1;
	int nextShotEntID = -1;
	float shotSpeed;
	int cooldownTime;
	int counter = 0;
};