#include "MovementS.hpp"



#include <iostream>

MovementS::MovementS()
{
	requiredComponents = { std::type_index(typeid(Transform)), std::type_index(typeid(Velocity)) };
}

void MovementS::update(float dt)
{
	for (auto entID : entities)
	{
		Entity ent = EntityManager::getInstance().getEntity(entID);
		
		Transform& trans = ent.getComponent<Transform>();
		Velocity& vel = ent.getComponent<Velocity>();

		trans.x += vel.x;
		trans.y += vel.y;
	}
}
