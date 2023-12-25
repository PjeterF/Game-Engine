#include "SpriteRenderingSystem.hpp"

SpriteRenderingSystem::SpriteRenderingSystem(RenderingAPI* API) : SystemBase(PAUSED)
{
	name = "SpriteRenderingSys(" + std::to_string(ID) + ")";

	requiredComponents = { Transform, Sprite };
	this->API = API;
}

void SpriteRenderingSystem::update(float dt)
{
	for (auto& entity : entities)
	{
		auto transform = (TransformC*)entity->getComponent(CType::Transform);
		auto sprite = (SpriteC*)entity->getComponent(CType::Sprite);

		API->drawSprite(transform->position, transform->size, transform->rotation, sprite->getTexture());
	}
}
