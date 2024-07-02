#pragma once

#include "../../src/ECS2/Systems/SysBase.hpp"
#include "../../src/ECS2/Components/Transform.hpp"
#include "../../src/ECS2/Components/Velocity.hpp"

class OrbitParentS : public SysBase
{
public:
	OrbitParentS();
	virtual void update(float dt) override;

	bool left = false;
	float speed = 2.0f;
	float radius = 100.0f;
	float margin = 5.0f;
};