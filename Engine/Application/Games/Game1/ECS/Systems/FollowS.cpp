#include "FollowS.hpp"

#include "../../src/ECS2/Components/Transform.hpp"
#include "../../src/ECS2/Components/Velocity.hpp"

#include <glm/glm.hpp>

#include <filesystem>
#include <iostream>
#include <string>

FollowS::FollowS(int targetID)
{
	requiredComponents = { std::type_index(typeid(Transform)), std::type_index(typeid(Velocity)) };
}

void FollowS::handleEvent(Event& event)
{
	SysBase::handleEvent(event);

	switch (event.getType())
	{
	case Event::EntityRemoval:
	{
		int* ID = (int*)event.getPayload();
		if (*ID == targetID)
			targetID = -1;
	}
	break;
	}
}

void FollowS::update(float dt)
{
	if (targetID < 0)
		return;

	auto transformPool = ComponentPoolManager::getInstance().getPool<Transform>();
	auto velocityPool = ComponentPoolManager::getInstance().getPool<Velocity>();

	auto& targetTransform = transformPool->get(targetID);
	auto& targetVelocity = velocityPool->get(targetID);

	for (auto entID : entities)
	{
		auto& transform = transformPool->get(entID);
		auto & velociy = velocityPool->get(entID);
		
		glm::vec2 direction(targetTransform.x + interceptStrength * targetVelocity.x - transform.x, targetTransform.y + interceptStrength * targetVelocity.y - transform.y);
		if (glm::length(direction) < 0.00001) {
			direction.x += rand() % 2 - 0.5f;
			direction.y += rand() % 2 - 0.5f;
		}
		if (glm::length(glm::vec2(targetTransform.x - transform.x, targetTransform.y - transform.y)) > rePositioningDistance)
		{
			glm::vec2 randomDir = { ((float)(rand() % 100) - 50) / 50, ((float)(rand() % 100) - 50) / 50 };
			if (glm::length(randomDir) < 0.001)
				randomDir.x = 1;
			randomDir =  glm::normalize(randomDir);
			transform.x = teleportDistance * randomDir.x + targetTransform.x;
			transform.y = teleportDistance * randomDir.y + targetTransform.y;
		}

		direction = (acceleration) * glm::normalize(direction);

		velociy.x = velociy.x + direction.x;
		velociy.y = velociy.y + direction.y;
	}
}

bool FollowS::setTarget(int ID)
{
	Entity targetEnt(ID);

	if (!targetEnt.isValid())
		return false;
	if (!targetEnt.hasComponent<Transform>())
		return false;
	if (!targetEnt.hasComponent<Velocity>())
		return false;

	targetID = ID;
	return true;
}
