#include "LayeredRenderingSystem.hpp"
#include "../Components/TransformC.hpp"
#include "../Components/RenderingLayerC.hpp"
#include "../Components/ParticleC.hpp"
#include "../Entity/EntManager.hpp"

LayeredRenderingSystem::LayeredRenderingSystem(RenderingAPI* API) : SystemBase(DRAWING, true, LayeredRenderingSys)
{
	name = "LayeredRenderingSystem(" + std::to_string(ID) + ")";

	requiredComponents = { Transform, RenderingLayer };
	this->API = API;
}

LayeredRenderingSystem& LayeredRenderingSystem::getInstance()
{
	return instanceInplementation();
}

void LayeredRenderingSystem::initialize(RenderingAPI* API)
{
	instanceInplementation(API);
}

void LayeredRenderingSystem::to_json(nlohmann::json& j) const
{
	j["type"] = type;
	j["entIDs"] = nlohmann::json::array();

	for (auto& ent : entities)
	{
		j["entIDs"].push_back(ent->getID());
	}
}

void LayeredRenderingSystem::from_json(nlohmann::json& j)
{
	for (auto& entID : j["entIDs"])
	{
		Ent* ent = EntManager::getInstance().getEntity(entID);
		if (ent != nullptr)
			LayeredRenderingSystem::getInstance().addEntity(ent);
	}
}

void LayeredRenderingSystem::update(float dt)
{
	for (auto& entity : entities)
	{
		auto layer = (RenderingLayerC*)entity->getComponent(RenderingLayer);

		if (entity->hasComponent(Sprite))
		{
			auto transform = (TransformC*)entity->getComponent(Transform);
			auto sprite = (SpriteC*)entity->getComponent(Sprite);

			//API->drawSprite(transform->position, transform->size, transform->rotation, sprite->getTexture());
			API->addSpriteInstance(transform->position, transform->size, transform->rotation, sprite->getTexture());
		}
		if (entity->hasComponent(AnimatedSprite))
		{
			auto transform = (TransformC*)entity->getComponent(Transform);
			auto aSprite = (AnimatedSpriteC*)entity->getComponent(AnimatedSprite);

			if (!aSprite->divisions.empty())
			{
				if (aSprite->counter >= aSprite->frameDuration)
				{
					aSprite->currentFrame = (aSprite->currentFrame + 1) % aSprite->divisions.size();
					aSprite->counter = 0;
				}

				//API->drawSpriteSampled(transform->position, transform->size, transform->rotation, aSprite->getTexture(), aSprite->divisions[aSprite->currentFrame]);

				TextureDivision div = aSprite->divisions[aSprite->currentFrame];
				API->addSpriteInstance(transform->position, transform->size, transform->rotation, aSprite->getTexture(), { div.x, div.y, div.width, div.height });

				if (!aSprite->paused)
					aSprite->counter++;
			}
		}
		API->drawSpriteInstances();
		if (entity->hasComponent(Particle))
		{
			auto transform = (TransformC*)entity->getComponent(Transform);
			auto emitter = (ParticleC*)entity->getComponent(Particle);

			emitter->emitter.draw(API);
		}
	}
}

bool LayeredRenderingSystem::addEntity(Ent* entity)
{
	 auto it = std::find(entities.begin(), entities.end(), entity);
	 if (it != entities.end())
		 return false;

	if (!validateComponents(entity))
		return false;

	if (entities.empty())
	{
		entities.push_back(entity);
		return true;
	}

	auto inserteeLayer = (RenderingLayerC*)entity->getComponent(RenderingLayer);

	for (auto it = entities.begin(); it != entities.end(); it++)
	{
		auto layer = (RenderingLayerC*)(*it)->getComponent(RenderingLayer);

		if (inserteeLayer->layer <= layer->layer )
		{
			entities.insert(it, entity);
			return true;
		}
	}

	entities.push_back(entity);
	return true;
}

void LayeredRenderingSystem::removeEntity(int ID)
{
	for (auto it = entities.begin(); it != entities.end(); it++)
	{
		if ((*it)->getID() == ID)
		{
			entities.erase(it);
			break;
		}
	}
}

void LayeredRenderingSystem::moveToLayer(int entID, int targetLayer)
{
	Ent* ent = EntManager::getInstance().getEntity(entID);

	if (ent == nullptr)
		return;

	if (!ent->hasComponent(RenderingLayer))
		return;

	this->removeEntity(entID);

	auto layer = (RenderingLayerC*)ent->getComponent(RenderingLayer);
	layer->layer = targetLayer;
	this->addEntity(ent);
}

void LayeredRenderingSystem::handleEvent(Event& event)
{
	SystemBase::handleEvent(event);

	switch (event.getType())
	{
	case Event::RenderingLayerChange:
	{
		int* ID = (int*)event.getPayload();
		removeEntity(*ID);
		addEntity(EntManager::getInstance().getEntity(*ID));
	}
	break;
	}
}

LayeredRenderingSystem& LayeredRenderingSystem::instanceInplementation(RenderingAPI* API)
{
	static LayeredRenderingSystem system(API);
	return system;
}
