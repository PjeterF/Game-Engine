#include "RouteManagementSystem.hpp"

#include "RouteInfoC.hpp"
#include <algorithm>
#include "../../src/ECS/Entity/EntManager.hpp"

std::vector<MarbleTemplate> RouteManagementSystem::marbleTemplates = std::vector<MarbleTemplate>();

MarbleTemplate::MarbleTemplate(float size, int tag, std::string textureFilepath)
{
	this->size = size;
	this->tag = tag;
	this->textureFilepath = textureFilepath;
}

RouteManagementSystem::RouteManagementSystem(CollisionSystem* collisionSystem, SpriteRenderingSystem* srs, MarbleCollisionResolutionSystem* marblecollisionSystem, std::vector<glm::vec2> pathPoints)
{
	name = "RouteSys(" + std::to_string(ID) + ")";

	requiredComponents = { Transform, Velocity, Sprite, BoxCollider, RouteInfo };
	this->pathPoints = pathPoints;
	this->collisionSystem = collisionSystem;
	this->spriteRenderingSystem = srs;
	this->marblecollisionSystem = marblecollisionSystem;
}

void RouteManagementSystem::update(float dt)
{
	if (remainingMarblesToSpawn != 0)
	{
		if (marbles.empty())
			spawnRandomMarble();
		else
		{
			auto first = marbles.begin();
			((RouteInfoC*)(*first)->getComponent(RouteInfo))->moving = true;
			auto transform = (TransformC*)(*first)->getComponent(Transform);

			if (glm::distance(pathPoints[0], transform->position) > 2 * transform->size.x && remainingMarblesToSpawn-- != 0)
				spawnRandomMarble();
		}
	}

	if (marbles.empty())
		return;

	moveRoutine(marbles.begin());
}

void RouteManagementSystem::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::EntityRemoval:
	{
		marbles.remove((Ent*)event.getPayload());
	}
	break;
	case Event::ComponentRemoval:
	{
		Ent* entity = (Ent*)event.getPayload();
		auto it = entities.find(entity->getID());
		if (it == entities.end())
			break;

		if (!validateComponents(entity))
			this->removeEntity(entity->getID());
	}
	break;
	}
}

void RouteManagementSystem::spawnRandomMarble()
{
	if (pathPoints.empty() || marbleTemplates.empty())
		return;

	int templateIndex = rand() % marbleTemplates.size();

	glm::vec2 pos = pathPoints[0];
	int size = 10;

	Ent* newMarble = EntManager::getInstance().createEntity();
	newMarble->addComponent(new BoxColliderC(pos.x, pos.y, 10, 10, newMarble));
	newMarble->addComponent(new TransformC(pos, { marbleTemplates[templateIndex].size, marbleTemplates[templateIndex].size }, 0));
	newMarble->addComponent(new VelocityC({ 0, 0 }));
	auto routeInfo = new RouteInfoC(marbleTemplates[templateIndex].tag, 1);
	routeInfo->route = this;
	newMarble->addComponent(routeInfo);
	newMarble->addComponent(new SpriteC(ResourceManager::getInstance().getResource<Texture>(marbleTemplates[templateIndex].textureFilepath)));

	marbles.push_front(newMarble);
	spriteRenderingSystem->addEntity(newMarble);
	collisionSystem->addEntity(newMarble);
	marblecollisionSystem->addEntity(newMarble);
}

std::list<Ent*>::iterator RouteManagementSystem::deleteMarble(std::list<Ent*>::iterator it)
{
	spriteRenderingSystem->removeEntity((*it)->getID());
	collisionSystem->removeEntity((*it)->getID());
	delete* it;
	return marbles.erase(it);
}

void RouteManagementSystem::moveRoutine(std::list<Ent*>::iterator oit)
{
	auto it = oit;
	while (it != marbles.end())
	{
		auto transform = (TransformC*)(*it)->getComponent(Transform);
		auto velocity = (VelocityC*)(*it)->getComponent(Velocity);
		auto routeInfo = (RouteInfoC*)(*it)->getComponent(RouteInfo);
		auto currCollider = (BoxColliderC*)(*it)->getComponent(BoxCollider);

		if (routeInfo->targetSample >= pathPoints.size())
			break;

		float threshold = 1;
		if (glm::distance(transform->position, pathPoints[routeInfo->targetSample]) < threshold)
		{
			routeInfo->targetSample++;
			if (routeInfo->targetSample >= pathPoints.size())
			{
				(*it)->destroy();
				break;
			}
		}

		for (auto collisionID : currCollider->collisionsIDs)
		{
			auto collision = collisionSystem->getCollision(collisionID);
			auto otherEntity = EntManager::getInstance().getEntity(collision->getOtherCollider(currCollider->ID)->entityID);
			auto otherRouteInfo = (RouteInfoC*)otherEntity->getComponent(RouteInfo);

			if (otherRouteInfo == nullptr)
				continue;

			if (otherRouteInfo->route == nullptr)
			{
				otherRouteInfo->route = routeInfo->route;
				otherRouteInfo->targetSample = routeInfo->targetSample;

				auto otherTransform = (TransformC*)otherEntity->getComponent(Transform);
				float forwardLength = glm::distance(otherTransform->position, transform->position + transform->position + velocity->velocity);
				float backwardsLength = glm::distance(otherTransform->position, transform->position + transform->position - velocity->velocity);

				EventManager::getInstance().notify(Event(Event::MarbleInsertion, otherEntity));
				auto it_insert = it;
				it_insert++;
				/*if (forwardLength < backwardsLength)
					it_insert++;*/

				marbles.insert(it_insert, otherEntity);
				if (popSame(it_insert, popThreshold) == 0)
					otherRouteInfo->inserting = true;
				else
					otherRouteInfo->inserting = false;
			}
		}

		if (it != oit)
		{
			auto it_prev = it;
			it_prev--;

			auto prev_Collider = (BoxColliderC*)(*it_prev)->getComponent(BoxCollider);
			auto prev_transform = (TransformC*)(*it_prev)->getComponent(Transform);
			auto prev_routeInfo = (RouteInfoC*)(*it_prev)->getComponent(RouteInfo);

			if (glm::distance(prev_transform->position, transform->position) < prev_transform->size.x + transform->size.x)
			{
				if (routeInfo->moving == false && prev_routeInfo->moving == true)
					popSame(it_prev, popThreshold);

				routeInfo->moving = true;
			}
			else
			{
				routeInfo->moving = false;
			}
		}
		else
		{
			routeInfo->moving = true;
		}

		if (routeInfo->moving)
		{
			velocity->velocity = marbleSpeed * glm::normalize(pathPoints[routeInfo->targetSample] - transform->position);
			transform->position = transform->position + velocity->velocity;
		}


		if (routeInfo->inserting)
		{
			routeInfo->inserting = false;

			auto it_prev = it;
			if(it_prev!=marbles.begin())
				it_prev--;

			auto prev_transform = (TransformC*)(*it_prev)->getComponent(Transform);
			auto prev_velocity = (VelocityC*)(*it_prev)->getComponent(Velocity);
			glm::vec2 nextPos = prev_transform->position + 2 * (prev_transform->size.x + transform->size.x) * glm::normalize(prev_velocity->velocity);
			float distance = glm::distance(transform->position, nextPos);
			int n_iterations = distance / marbleSpeed;

			for(int i=0 ; i< n_iterations ; i++)
				moveRoutine(it);
		}
		it++;
	}
}

int RouteManagementSystem::popSame(std::list<Ent*>::iterator it, int threshold)
{
	if (it == marbles.end())
		return 0;
	
	auto ri = (RouteInfoC*)(*it)->getComponent(RouteInfo);
	int tag = ri->tag;

	auto it_forward = it;
	it_forward++;
	auto it_backwards = it;

	std::vector<Ent*> marblesToPop;
	int n = 0;

	while (it_forward != marbles.end())
	{
		auto routeInfo = (RouteInfoC*)(*it_forward)->getComponent(RouteInfo);
		if (routeInfo->tag == tag && ri->moving == routeInfo->moving)
		{
			marblesToPop.push_back(*it_forward);
			n++;
		}
		else
			break;

		it_forward++;
	}

	while (it_backwards != marbles.begin())
	{
		auto routeInfo = (RouteInfoC*)(*it_backwards)->getComponent(RouteInfo);
		if (routeInfo->tag == tag && ri->moving == routeInfo->moving)
		{
			marblesToPop.push_back(*it_backwards);
			n++;
		}
		else
			break;

		it_backwards--;
	}

	if (it_backwards == marbles.begin())
	{
		auto routeInfo = (RouteInfoC*)(*it_backwards)->getComponent(RouteInfo);
		if (routeInfo->tag == tag && ri->moving == routeInfo->moving)
		{
			marblesToPop.push_back(*it_backwards);
			n++;
		}
	}

	if (n >= threshold)
	{
		for (auto marble : marblesToPop)
		{
			marble->destroy();
		}
		return marblesToPop.size();
	}

	return 0;
}
