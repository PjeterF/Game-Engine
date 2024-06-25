#include "FollowS.hpp"

#include "../../src/ECS2/Components/Transform.hpp"
#include "../../src/ECS2/Components/Velocity.hpp"

#include <glm/glm.hpp>

FollowS::FollowS(int targetID)
{
	requiredComponents = { std::type_index(typeid(Transform)), std::type_index(typeid(Velocity)) };
}

void FollowS::update(float dt)
{
	if (targetID < 0)
		return;

	auto transformPool = ComponentPoolManager::getInstance().getPool<Transform>();
	auto velocityPool = ComponentPoolManager::getInstance().getPool<Velocity>();

	auto& targetTransform = transformPool->get(targetID);

	for (auto entID : entities)
	{
		auto& transform = transformPool->get(entID);
		auto & velociy = velocityPool->get(entID);

		float velLength = glm::length(glm::vec2(velociy.x, velociy.y));

		glm::vec2 newVelocity = (velLength+acceleration) * glm::normalize(
			glm::vec2(targetTransform.x, targetTransform.y)
			-
			glm::vec2(transform.x, transform.y)
		);

		velociy.x = newVelocity.x;
		velociy.y = newVelocity.y;
	}
}

bool FollowS::setTarget(int ID)
{
	Entity targetEnt(ID);

	if (!targetEnt.isValid())
		return false;
	if (!targetEnt.hasComponent<Transform>())
		return false;

	targetID = ID;
	return true;
}
