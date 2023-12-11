#include "AnimatedSpriteSystem.hpp"
#include "../Components/TransformC.hpp"
#include "../Components/AnimatedSpriteC.hpp"

AnimatedSpriteSystem::AnimatedSpriteSystem(RenderingAPI* API)
{
	name = "AnimatedSpriteRenderingSys(" + std::to_string(ID) + ")";

	requiredComponents = { Transform, AnimatedSprite };
	this->API = API;
}

void AnimatedSpriteSystem::update(float dt)
{
	for (auto& entity : entities)
	{
		auto transform = (TransformC*)entity.second->getComponent(Transform);
		auto aSprite = (AnimatedSpriteC*)entity.second->getComponent(AnimatedSprite);

		if (aSprite->counter == aSprite->frameDuration)
		{
			aSprite->currentFrame = (aSprite->currentFrame + 1) % aSprite->divisions.size();
			aSprite->counter = 0;
		}

		API->drawSpriteSampled(transform->position, transform->size, transform->rotation, aSprite->getTexture(), aSprite->divisions[aSprite->currentFrame]);

		aSprite->counter++;
	}
}