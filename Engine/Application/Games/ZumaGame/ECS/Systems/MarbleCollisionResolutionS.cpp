#include "MarbleCollisionResolutionS.hpp"

#include "../../src/ECS2/SystemsManager.hpp"

MarbleCollisionResolutionS::MarbleCollisionResolutionS()
{
	requiredComponents = {
		std::type_index(typeid(Transform)),
		std::type_index(typeid(AABB)),
		std::type_index(typeid(MarbleComponent))
	};
}

void MarbleCollisionResolutionS::update(float dt)
{
	auto marbleCPool = ComponentPoolManager::getInstance().getPool<MarbleComponent>();
	auto colPool = ComponentPoolManager::getInstance().getPool<AABB>();
	auto velPool = ComponentPoolManager::getInstance().getPool<Velocity>();

	std::unordered_map<std::string, SysBase*>& routes = SystemsManager::getInstance().getSystemBin<RouteS>();

	for (auto ID : entities)
	{
		auto& collider = colPool->get(ID);
		auto& marbleC = marbleCPool->get(ID);

		for (auto colID : collider.collidingEntIDs)
		{
			if (marbleCPool->entityHasComponent[colID])
			{
				auto& otherMarbleC = marbleCPool->get(colID);

				//Shot on shot collision
				if (otherMarbleC.routeName == "")
					continue;

				auto it = routes.find(otherMarbleC.routeName);
				if (it != routes.end())
				{
					RouteS* otherRoute = (RouteS*)(*it).second;
					auto& otherVel = velPool->get(colID);

					otherRoute->insertAt(colID, ID);
					entitiesToRemove.push_back(ID);
				}
			}
		}
	}

	for (auto ID : entitiesToRemove)
	{
		this->removeEntity(ID);
	}
	entitiesToRemove.clear();
}
