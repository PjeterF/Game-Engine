#include "TestCollisionResponse.hpp"

TestCollisionResponse::TestCollisionResponse(CollisionSystem* collisionSystem)
{
	this->requiredComponents = { Transform, BoxCollider, Velocity };
	this->collisionSystem = collisionSystem;
}

void TestCollisionResponse::update(float dt)
{
	for (auto& entity : entities)
	{
		auto transform = (TransformC*)entity.second->getComponent(CType::Transform);
		auto velocity = (VelocityC*)entity.second->getComponent(CType::Velocity);
		auto collider = (BoxColliderC*)entity.second->getComponent(CType::BoxCollider);

		for (auto& collisionID : collider->collisionsIDs)
		{
			auto collision = collisionSystem->getCollision(collisionID);
			if (collision != nullptr)
			{
				if (collision->state == CollisionECS::Enter)
					velocity->velocity = -velocity->velocity;
			}
		}
	}
}
