#include "MovementS.hpp"

#include <glm/glm.hpp>

MovementS::MovementS()
{
	requiredComponents = { std::type_index(typeid(Transform)), std::type_index(typeid(Velocity)) };
}

void MovementS::update(float dt)
{
	auto transformPool = ComponentPoolManager::getInstance().getPool<Transform>();
	auto velocityPool = ComponentPoolManager::getInstance().getPool<Velocity>();

	for (auto entID : entities)
	{
		Transform& trans = transformPool->get(entID);
		Velocity& vel = velocityPool->get(entID);

		trans.x += dt * vel.x;
		trans.y += dt * vel.y;

		vel.x += vel.ax;
		vel.y += vel.ay;

		vel.x = vel.x * vel.decay;
		vel.y = vel.y * vel.decay;

		if (trans.rotateToDir)
		{
			glm::vec2 velVec = { vel.x, vel.y };
			if (glm::length(velVec) > 0.0001)
			{
				glm::vec2 dir = glm::normalize(velVec);
				trans.rot = atan2(-dir.y, dir.x);
				if (dir.x < 0)
					trans.rot += 3.14;
			}
		}
	}
}