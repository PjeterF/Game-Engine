#pragma once

#include "../../src/Graphics/GeneralRenderer.hpp"

#include "../Components/CharacterStats.hpp"
#include "../../src/ECS2/Components/Transform.hpp"
#include "../../src/ECS2/Components/AABB.hpp"

#include "../../src/ECS2/Systems/SysBase.hpp"

class CollisionResolutionS : public SysBase
{
public:
	CollisionResolutionS();
	virtual void update(float dt) override;
	void drawHealthBars(RenderingAPI* API);
};