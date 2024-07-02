#include "OrbitParentS.hpp"

#include <glm/glm.hpp>

OrbitParentS::OrbitParentS()
{
	requiredComponents = {
		std::type_index(typeid(Transform)),
		//std::type_index(typeid(ParentEntity)),
		std::type_index(typeid(Velocity))
	};
}

void OrbitParentS::update(float dt)
{
	/*auto transPool = ComponentPoolManager::getInstance().getPool<Transform>();
	auto parentPool = ComponentPoolManager::getInstance().getPool<ParentEntity>();
	auto velPool = ComponentPoolManager::getInstance().getPool<Velocity>();

	for (auto ID : entities)
	{
		auto& parentC = parentPool->get(ID);
		if (parentC.parentID == -1)
			continue;

		auto& parentTrans = transPool->get(parentC.parentID);
		auto& trans = transPool->get(ID);
		auto& vel = velPool->get(ID);

		glm::vec2 rVec = glm::normalize(glm::vec2(trans.x, trans.y));

		trans.x = radius * rVec.x;
		trans.y = radius * rVec.y;

		glm::vec2 dir = { rVec.y, -rVec.x };

		vel.x += speed * dir.x;
		vel.y += speed * dir.y;
	}*/
}
