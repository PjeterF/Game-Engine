#include "LayeredRenderingSystem.hpp"
#include "../Components/TransformC.hpp"
#include "../Components/RenderingLayerC.hpp"
#include "../Entity/EntManager.hpp"

LayeredRenderingSystem::LayeredRenderingSystem(RenderingAPI* API) : SystemBase(PAUSED)
{
	name = "LayeredRenderingSystem(" + std::to_string(ID) + ")";

	requiredComponents = { Transform, RenderingLayer };
	this->API = API;
}

void LayeredRenderingSystem::update(float dt)
{
	for (auto& entity : orderedEntities)
	{
		auto layer = (RenderingLayerC*)entity->getComponent(RenderingLayer);

		if (entity->hasComponent(Sprite))
		{
			auto transform = (TransformC*)entity->getComponent(Transform);
			auto sprite = (SpriteC*)entity->getComponent(Sprite);

			API->drawSprite(transform->position, transform->size, transform->rotation, sprite->getTexture());
		}
		else if (entity->hasComponent(AnimatedSprite))
		{
			auto transform = (TransformC*)entity->getComponent(Transform);
			auto aSprite = (AnimatedSpriteC*)entity->getComponent(AnimatedSprite);

			if (aSprite->counter >= aSprite->frameDuration)
			{
				aSprite->currentFrame = (aSprite->currentFrame + 1) % aSprite->divisions.size();
				aSprite->counter = 0;
			}

			API->drawSpriteSampled(transform->position, transform->size, transform->rotation, aSprite->getTexture(), aSprite->divisions[aSprite->currentFrame]);

			if (!aSprite->paused)
				aSprite->counter++;
		}
	}
}

bool LayeredRenderingSystem::addEntity(Ent* entity)
{
	 auto it = std::find(orderedEntities.begin(), orderedEntities.end(), entity);
	 if (it != orderedEntities.end())
		 return false;

	if (!validateComponents(entity))
		return false;

	if (orderedEntities.empty())
	{
		orderedEntities.push_back(entity);
		return true;
	}

	auto inserteeLayer = (RenderingLayerC*)entity->getComponent(RenderingLayer);

	for (auto it = orderedEntities.begin(); it != orderedEntities.end(); it++)
	{
		auto layer = (RenderingLayerC*)(*it)->getComponent(RenderingLayer);

		if (inserteeLayer->layer <= layer->layer)
		{
			orderedEntities.insert(it, entity);
			return true;
		}
	}
}

void LayeredRenderingSystem::removeEntity(int ID)
{
	for (auto it = orderedEntities.begin(); it != orderedEntities.end(); it++)
	{
		if ((*it)->getID() == ID)
		{
			orderedEntities.erase(it);
			break;
		}
	}
}

void LayeredRenderingSystem::handleEvent(Event& event)
{
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

	SystemBase::handleEvent(event);
}
