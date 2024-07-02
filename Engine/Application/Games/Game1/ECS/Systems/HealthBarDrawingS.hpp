#pragma once

#include "../../src/Graphics/GeneralRenderer.hpp"

#include "../../src/ECS2/Systems/SysBase.hpp"

#include "../../src/ECS2/Components/Transform.hpp"
#include "../../src/ECS2/Components/AABB.hpp"
#include "../Components/CharacterStats.hpp"

class HealthBarDrawingS : public SysBase
{
public:
	HealthBarDrawingS(RenderingAPI* rAPI);
	virtual void update(float dt) override;

	glm::vec2 barDim = { 50, 5 };
private:
	RenderingAPI* rAPI;
};