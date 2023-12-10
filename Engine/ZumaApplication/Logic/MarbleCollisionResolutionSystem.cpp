#include "MarbleCollisionResolutionSystem.hpp"
#include "../../src/ECS/Entity/EntManager.hpp"
#include "../../src/ECS/Components/BoxColliderC.hpp"
#include "RouteInfoC.hpp"

MarbleCollisionResolutionSystem::MarbleCollisionResolutionSystem(CollisionSystem* collisionSystem)
{
	requiredComponents = { RouteInfo, BoxCollider };
	collisionSystem = collisionSystem;
}

void MarbleCollisionResolutionSystem::update(float dt)
{
	for (auto& entity : entities)
	{
		auto collider = (BoxColliderC*)entity.second->getComponent(BoxCollider);
		auto routeInfo = (RouteInfoC*)entity.second->getComponent(RouteInfo);

		for (auto collisionID : collider->collisionsIDs)
		{
			/*auto collision = collisionSystem->getCollision(collisionID);

			auto otherCollider = collision->getOtherCollider(collider->ID);
			auto otherEntity = EntManager::getInstance().getEntity(otherCollider->entityID);
			auto otherRouteInfo = (RouteInfoC*)otherEntity->getComponent(RouteInfo);
			if (otherRouteInfo == nullptr)
				continue;

			if (otherRouteInfo->route==nullptr && routeInfo->route!=nullptr)
			{
				otherRouteInfo->route = routeInfo->route;
				otherRouteInfo->targetSample = routeInfo->targetSample;
				routeInfo->route->insertAfter(entity.second->getID(), otherEntity);
			}*/
		}
	}
}
