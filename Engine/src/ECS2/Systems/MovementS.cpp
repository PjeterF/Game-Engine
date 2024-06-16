#include "MovementS.hpp"

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

		vel.x = vel.x * 0.9;
		vel.y = vel.y * 0.9;

		vel.x += vel.ax;
		vel.y += vel.ay;

		trans.x += vel.x;
		trans.y += vel.y;
	}
}
