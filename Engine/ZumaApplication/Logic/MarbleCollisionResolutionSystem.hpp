#pragma once

#include "../../src/ECS/Systems/SystemBase.hpp"
#include "../../src/ECS/Systems/CollisionSystem.hpp"

class MarbleCollisionResolutionSystem : public SystemBase
{
public:
	MarbleCollisionResolutionSystem(CollisionSystem* collisionSystem);
	virtual void update(float dt) override;
private:
	CollisionSystem* collisionSystem;
};