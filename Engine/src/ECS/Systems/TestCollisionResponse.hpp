#pragma once

#include "SystemBase.hpp"
#include "CollisionSystem.hpp"

#include "../Components/VelocityC.hpp"
#include "../Components/TransformC.hpp"
#include "../Components/BoxColliderC.hpp"

class TestCollisionResponse : public SystemBase
{
public:
	TestCollisionResponse(CollisionSystem* collisionSystem);
	virtual void update(float dt) override;
private:
	CollisionSystem* collisionSystem;
};