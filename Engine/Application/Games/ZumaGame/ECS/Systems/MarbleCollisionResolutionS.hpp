#pragma once

#include "../../src/ECS2/Systems/SysBase.hpp"

#include <string>

#include "../../src/ECS2/Components/Transform.hpp"
#include "../../src/ECS2/Components/Velocity.hpp"
#include "../../src/ECS2/Components/AABB.hpp"
#include "../Components/MarbleComponent.hpp"

#include "../../ECS/Systems/RouteS.hpp"

class MarbleCollisionResolutionS : public SysBase
{
public:
	MarbleCollisionResolutionS();
	virtual void update(float dt) override;
private:
	std::vector<int> entitiesToRemove;
};