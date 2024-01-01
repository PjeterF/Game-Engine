#include "RouteManagementSystem.hpp"

#include "RouteInfoC.hpp"
#include <algorithm>
#include "../../src/ECS/Entity/EntManager.hpp"
#include "../../src/ECS/Systems/LayeredRenderingSystem.hpp"

std::vector<MarbleTemplate> RouteManagementSystem::marbleTemplates = std::vector<MarbleTemplate>();

MarbleTemplate::MarbleTemplate(float size, int tag, std::string textureFilepath, std::vector<TextureDivision> divisions, int frameDuration)
{
	this->size = size;
	this->tag = tag;
	this->textureFilepath = textureFilepath;
	this->divisions = divisions;
	this->frameDuration = frameDuration;
}

RouteManagementSystem::RouteManagementSystem(Spline* spline, int initialMarbleNumber) : SystemBase(UNPAUSED, false, RouteManagementSys)
{
	name = "RouteSys(" + std::to_string(ID) + ")";

	requiredComponents = { Transform, Velocity, AnimatedSprite, BoxCollider, RouteInfo };
	this->spline = spline;

	setInitialMarbleCount(initialMarbleNumber);

	EventManager::getInstance().notify(Event(Event::RouteCreation, this), UI);
}

RouteManagementSystem::~RouteManagementSystem()
{
	for (auto& marble : marbles)
	{
		marble->destroy();
	}
	EventManager::getInstance().notify(Event(Event::RouteDeletion, this), UI);
}

void RouteManagementSystem::to_json(nlohmann::json& j) const
{
	j["type"] = type;
	j["controlPoints"] = nlohmann::json::array();
	for (auto point : *spline->getControlPoints())
		j["controlPoints"].push_back({ point.x, point.y });

	j["marbleSpeed"] = marbleSpeed;
	j["popThreshold"] = popThreshold;
	j["maxMarbles"] = maxMarbles;
	j["layer"] = layer;
}

void RouteManagementSystem::from_json(nlohmann::json& j)
{
	std::vector<glm::vec2> ctrlPoints;
	for (auto& point : j["controlPoints"])
		ctrlPoints.push_back({ point[0], point[1] });

	delete spline;
	spline = new Spline
		(ctrlPoints, 100,
			ResourceManager::getInstance().getResource<Texture>("src/textures/control_point.png"),
			ResourceManager::getInstance().getResource<Texture>("src/textures/control_point2.png"),
			5
		);

	this->marbleSpeed = j["marbleSpeed"];
	this->popThreshold = j["popThreshold"];
	this->maxMarbles = j["maxMarbles"];
	this->marblesLeftToSpawn = j["maxMarbles"];
	this->maxMarbles = j["layer"];
}

void RouteManagementSystem::update(float dt)
{
	if (marblesLeftToSpawn != 0)
	{
		if (entities.empty())
			spawnRandomMarble();
		else if(marblesLeftToSpawn>0)
		{
			auto first = entities.end();
			first--;
			((RouteInfoC*)(*first)->getComponent(RouteInfo))->moving = true;
			auto transform = (TransformC*)(*first)->getComponent(Transform);

			if (glm::distance((*spline->getSampledPoints())[0], transform->position) > 2 * transform->size.x)
				spawnRandomMarble();
		}
	}

	if (entities.empty())
		return;

	moveRoutine2(--entities.end());
}

void RouteManagementSystem::handleEvent(Event& event)
{
	SystemBase::handleEvent(event);

	switch (event.getType())
	{
	//case Event::ComponentRemoval:
	//{
	//	Ent* entity = (Ent*)event.getPayload();
	//	auto it = std::find(entities.begin(), entities.end(), entity);
	//	if (it == entities.end())
	//		break;

	//	if (!validateComponents(entity))
	//		this->removeEntity(entity->getID());
	//}
	//break;
	case Event::MoveCtrlPoints:
	{
		if (spline == event.getPayload())
			EventManager::getInstance().notify(Event(Event::RouteSelection, this), UI);
	}
	break;
	}
}

void RouteManagementSystem::spawnRandomMarble()
{
	if ((*spline->getSampledPoints()).empty() || marbleTemplates.empty())
		return;

	int templateIndex = rand() % marbleTemplates.size();

	glm::vec2 pos = (*spline->getSampledPoints())[0];
	int size = 10;

	Ent* newMarble = EntManager::getInstance().createEntity();
	newMarble->addComponent(new BoxColliderC(pos.x, pos.y, 10, 10, newMarble));
	newMarble->addComponent(new TransformC(pos, { marbleTemplates[templateIndex].size, marbleTemplates[templateIndex].size }, 0));
	newMarble->addComponent(new VelocityC({ 0, 0 }));
	newMarble->addComponent(new RenderingLayerC(this->layer));
	auto routeInfo = new RouteInfoC(marbleTemplates[templateIndex].tag, 1);
	routeInfo->route = this;
	newMarble->addComponent(routeInfo);
	newMarble->addComponent(new AnimatedSpriteC(ResourceManager::getInstance().getResource<Texture>(marbleTemplates[templateIndex].textureFilepath), marbleTemplates[templateIndex].divisions, 30));

	LayeredRenderingSystem::getInstance().addEntity(newMarble);
	CollisionSystem::getInstance().addEntity(newMarble);

	entities.push_back(newMarble);
	marblesLeftToSpawn--;
}

void RouteManagementSystem::setLayer(int target)
{
	this->layer = target;
}

void RouteManagementSystem::setInitialMarbleCount(int n)
{
	maxMarbles = n;
	marblesLeftToSpawn = n;
}

int RouteManagementSystem::getInitialMarbleCount()
{
	return maxMarbles;
}

void RouteManagementSystem::drawSpline(RenderingAPI* API)
{
	spline->draw(API);
}

void RouteManagementSystem::addRoutePoint(glm::vec2 pos)
{
	spline->addSegment(pos.x, pos.y);
}

void RouteManagementSystem::removeLastRoutePoint()
{
	spline->removeLastSegment();
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

		if (routeInfo->targetSample >= (*spline->getSampledPoints()).size())
			break;

		float threshold = 1;
		if (glm::distance(transform->position, (*spline->getSampledPoints())[routeInfo->targetSample]) < threshold)
		{
			routeInfo->targetSample++;
			if (routeInfo->targetSample >= (*spline->getSampledPoints()).size())
			{
				(*it)->destroy();
				break;
			}
		}

		for (auto collisionID : currCollider->collisionsIDs)
		{
			auto collision = CollisionSystem::getInstance().getCollision(collisionID);
			auto otherEntity = EntManager::getInstance().getEntity(collision->getOtherCollider(currCollider->ID)->entityID);
			auto otherRouteInfo = (RouteInfoC*)otherEntity->getComponent(RouteInfo);

			if (otherRouteInfo == nullptr)
				continue;

			if (otherRouteInfo->route == nullptr)
			{
				otherRouteInfo->route = routeInfo->route;
				otherRouteInfo->targetSample = routeInfo->targetSample+1;

				auto otherTransform = (TransformC*)otherEntity->getComponent(Transform);
				float forwardLength = glm::distance(otherTransform->position, transform->position + transform->position + velocity->velocity);
				float backwardsLength = glm::distance(otherTransform->position, transform->position + transform->position - velocity->velocity);

				EventManager::getInstance().notify(Event(Event::MarbleInsertion, otherEntity), ECS);
				auto it_insert = it;
				it_insert++;
				/*if (forwardLength < backwardsLength)
					it_insert++;*/

				marbles.insert(it_insert, otherEntity);
				/*if (popSame(it_insert, popThreshold) == 0)
					otherRouteInfo->inserting = true;
				else
					otherRouteInfo->inserting = false;*/
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
					//popSame(it_prev, popThreshold);

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
			velocity->velocity = marbleSpeed * glm::normalize((*spline->getSampledPoints())[routeInfo->targetSample] - transform->position);
			transform->position = transform->position + velocity->velocity;
			transform->rotation = 57.2958 * atan(velocity->velocity.y / velocity->velocity.x);
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

void RouteManagementSystem::moveRoutine2(std::vector<Ent*>::iterator s_it)
{
	auto it = s_it;
	while(1)
	{
		auto transform = (TransformC*)(*it)->getComponent(Transform);
		auto velocity = (VelocityC*)(*it)->getComponent(Velocity);
		auto routeInfo = (RouteInfoC*)(*it)->getComponent(RouteInfo);
		auto currCollider = (BoxColliderC*)(*it)->getComponent(BoxCollider);

		if (routeInfo->targetSample >= (*spline->getSampledPoints()).size())
			break;

		float threshold = 1;
		if (glm::distance(transform->position, (*spline->getSampledPoints())[routeInfo->targetSample]) < threshold)
		{
			routeInfo->targetSample++;
			if (routeInfo->targetSample >= (*spline->getSampledPoints()).size())
			{
				(*it)->destroy();
				break;
			}
		}

		if (it != s_it)
		{
			auto it_prev = it;
			it_prev++;
			
			auto prev_Collider = (BoxColliderC*)(*it_prev)->getComponent(BoxCollider);
			auto prev_transform = (TransformC*)(*it_prev)->getComponent(Transform);
			auto prev_routeInfo = (RouteInfoC*)(*it_prev)->getComponent(RouteInfo);

			float distance = glm::distance(prev_transform->position, transform->position);
			if (distance < prev_transform->size.x + transform->size.x)
			{
				if (routeInfo->separated == true && prev_routeInfo->separated == false)
				{
					auto it2 = it;
					if (it2 != entities.begin())
						it2--;

					while (1)
					{
						auto routeInfo = (RouteInfoC*)(*it2)->getComponent(RouteInfo);

						routeInfo->separated = false;

						if (it2 == entities.begin())
							break;
						else
							it2--;
					}

					routeInfo->moving = true;

					if(popSame(it2, popThreshold))
						return;
				}


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

		for (auto colID : currCollider->collisionsIDs)
		{
			auto otherCol = CollisionSystem::getInstance().getCollision(colID);
			auto otherEntity = EntManager::getInstance().getEntity(otherCol->getOtherCollider(currCollider->ID)->entityID);
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

				EventManager::getInstance().notify(Event(Event::MarbleInsertion, otherEntity), ECS);

				otherRouteInfo->inserting = true;
				it = entities.insert(++it, otherEntity);
				popSame(it, 3);
				/*if (popSame(it_insert, popThreshold) == 0)
					otherRouteInfo->inserting = true;
				else
					otherRouteInfo->inserting = false;*/
				return;
			}
		}

		if (routeInfo->moving)
		{
			velocity->velocity = marbleSpeed * glm::normalize((*spline->getSampledPoints())[routeInfo->targetSample] - transform->position);
			transform->position = transform->position + velocity->velocity;
			transform->rotation = 57.2958 * atan(velocity->velocity.y / velocity->velocity.x);

			if (routeInfo->separated)
				transform->rotation = 0;
		}

		if (routeInfo->inserting)
		{
			routeInfo->inserting = false;

			auto it_prev = it;
			it_prev++;

			auto prev_transform = (TransformC*)(*it_prev)->getComponent(Transform);
			auto prev_velocity = (VelocityC*)(*it_prev)->getComponent(Velocity);
			glm::vec2 nextPos = prev_transform->position + 2 * (prev_transform->size.x + transform->size.x) * glm::normalize(prev_velocity->velocity);
			float distance = glm::distance(transform->position, nextPos);
			int n_iterations = distance / marbleSpeed;

			for (int i = 0; i < n_iterations; i++)
				moveRoutine2(it);
		}

		if (it == entities.begin())
			return;
		else
			it--;
	}
}

int RouteManagementSystem::popSame(std::vector<Ent*>::iterator it, int threshold)
{
	if (it == entities.end())
		return 0;
	
	auto ri = (RouteInfoC*)(*it)->getComponent(RouteInfo);
	int tag = ri->tag;

	auto it_forward = it;
	it_forward++;
	auto it_backwards = it;

	std::vector<Ent*> marblesToPop;
	int n = 0;

	while (it_forward != entities.end())
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

	while (it_backwards != entities.begin())
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

	if (it_backwards == entities.begin())
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
		while (1)
		{
			auto routeInfo = (RouteInfoC*)(*it_backwards)->getComponent(RouteInfo);

			routeInfo->separated = true;

			if (it_backwards == entities.begin())
				break;
			else
				it_backwards--;
		}

		for (auto marble : marblesToPop)
		{
			marble->destroy();
		}
		return marblesToPop.size();
	}

	return 0;
}
