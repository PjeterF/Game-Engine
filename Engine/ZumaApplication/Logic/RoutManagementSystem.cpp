#include "RouteManagementSystem.hpp"

#include "RouteInfoC.hpp"
#include <algorithm>
#include "../../src/ECS/Entity/EntManager.hpp"

#include "../../src/ECS/Systems/LayeredRenderingSystem.hpp"
#include "../../src/ECS/Systems/ParticleSystem.hpp"
#include "../../src/ECS/Systems/CounterKillerSystem.hpp"

#include "../../src/ECS/Components/ParticleC.hpp"
#include "../../src/ECS/Components/CounterC.hpp"

std::vector<MarbleTemplate> RouteManagementSystem::marbleTemplates = std::vector<MarbleTemplate>();

MarbleTemplate::MarbleTemplate(float size, int tag, std::string textureFilepath, std::vector<TextureDivision> divisions, int frameDuration, glm::vec3 explosionColor)
{
	this->size = size;
	this->tag = tag;
	this->textureFilepath = textureFilepath;
	this->divisions = divisions;
	this->frameDuration = frameDuration;
	this->explosionColor = explosionColor;
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

	moveRoutine(--entities.end());
}

void RouteManagementSystem::handleEvent(Event& event)
{
	SystemBase::handleEvent(event);

	switch (event.getType())
	{
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

void RouteManagementSystem::spawnExplosion(float x, float y, glm::vec3 startColor, glm::vec3 endColor)
{
	Ent* explosion = EntManager::getInstance().createEntity();

	int maxLifetime = 50;

	ParticleC* emitter = new ParticleC(x, y, 15, 100000, 15);
	emitter->emitter.defaultProperties.startColour = { startColor.r, startColor.g, startColor.b, 1 };
	emitter->emitter.defaultProperties.endColour = { endColor.r, endColor.g, endColor.b, 1 };
	emitter->emitter.defaultProperties.startSize = 3;
	emitter->emitter.defaultProperties.endSize = 0;
	emitter->emitter.defaultProperties.xVelVar = { -5, 5 };
	emitter->emitter.defaultProperties.yVelVar = { -5, 5 };
	emitter->emitter.defaultProperties.velocityDecay = 0.9;
	emitter->emitter.defaultProperties.particleLifetime = { maxLifetime * 0.9, maxLifetime };

	explosion->addComponent(emitter);
	explosion->addComponent(new TransformC({x, y}));
	explosion->addComponent(new RenderingLayerC(this->layer));
	explosion->addComponent(new CounterC(maxLifetime));

	CounterKillerSystem::getInstance().addEntity(explosion);
	ParticleSystem::getInstance().addEntity(explosion);
	LayeredRenderingSystem::getInstance().addEntity(explosion);
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

void RouteManagementSystem::moveRoutine(std::vector<Ent*>::iterator s_it)
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

					if (routeInfo->tag == prev_routeInfo->tag)
					{
						if (popSame(it2, popThreshold) || popSame(it_prev, popThreshold))
							return;
					}
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

			int n_iterations;

			auto it_prev = it;
			it_prev++;

			if (it_prev == entities.end())
			{
				n_iterations = 1;
			}
			else
			{
				auto prev_transform = (TransformC*)(*it_prev)->getComponent(Transform);
				auto prev_velocity = (VelocityC*)(*it_prev)->getComponent(Velocity);
				glm::vec2 nextPos = prev_transform->position + 2 * (prev_transform->size.x + transform->size.x) * glm::normalize(prev_velocity->velocity);
				float distance = glm::distance(transform->position, nextPos);
				n_iterations = distance / marbleSpeed;
			}

			for (int i = 0; i < n_iterations; i++)
				moveRoutine(it);
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
		if (routeInfo->tag == tag && ri->separated == routeInfo->separated)
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
		if (routeInfo->tag == tag && ri->separated == routeInfo->separated)
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
		if (routeInfo->tag == tag && ri->separated == routeInfo->separated)
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
			auto transform = (TransformC*)marble->getComponent(Transform);
			auto routeInfo = (RouteInfoC*)marble->getComponent(RouteInfo);
			
			for (auto& temp : marbleTemplates)
			{
				if (temp.tag == routeInfo->tag)
				{
					spawnExplosion(transform->position.x, transform->position.y, temp.explosionColor, 0.2f * temp.explosionColor);
					spawnExplosion(transform->position.x, transform->position.y, { 0.1, 0.1, 0.1 }, { 0, 0, 0 });
					break;
				}
			}

			marble->destroy();
		}

		return marblesToPop.size();
	}

	return 0;
}
