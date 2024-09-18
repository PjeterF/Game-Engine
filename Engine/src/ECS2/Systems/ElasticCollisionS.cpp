#include "ElasticCollisionS.hpp"
#include "CollisionS.hpp"
#include "../SystemsManager.hpp"

ElasticCollisionS::ElasticCollisionS()
{
	requiredComponents = {
		std::type_index(typeid(Transform)),
		std::type_index(typeid(AABB)),
		std::type_index(typeid(Velocity))
	};
}

void ElasticCollisionS::update(float dt)
{
	auto tPool = ComponentPoolManager::getInstance().getPool<Transform>();
	auto vPool = ComponentPoolManager::getInstance().getPool<Velocity>();
	auto cPool = ComponentPoolManager::getInstance().getPool<AABB>();

	auto cSys = SystemsManager::getInstance().getSystem<CollisionS>();

	for (auto ID1 : entities)
	{
		auto& transform1 = tPool->get(ID1);
		auto& velocity1 = vPool->get(ID1);
		auto& collider1 = cPool->get(ID1);

		for (auto ID2 : collider1.collidingEntIDs)
		{
			if (ID1 > ID2)
				continue;

			if (entities.find(ID2) == entities.end())
				continue;

			if (cSys->getCollision(ID1, ID2).state != CollisionS::Collision::State::entry)
				continue;

			auto& transform2 = tPool->get(ID2);
			auto& velocity2 = vPool->get(ID2);
			auto& collider2 = cPool->get(ID2);

			glm::vec2 u1(velocity1.x, velocity1.y), u2(velocity2.x, velocity2.y);

			glm::vec2 newV1 = ((collider1.mass - collider2.mass) * u1 + 2 * collider2.mass * u2) / (collider1.mass + collider2.mass);
			glm::vec2 newV2 = ((collider2.mass - collider1.mass) * u2 + 2 * collider1.mass * u1) / (collider1.mass + collider2.mass);
		
			velocity1.x = newV1.x;
			velocity1.y = newV1.y;

			velocity2.x = newV2.x;
			velocity2.y = newV2.y;
		}
	}
}
