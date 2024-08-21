#include "FlipToXDirectionS.hpp"

FlipSpriteToXDirectionS::FlipSpriteToXDirectionS()
{
	requiredComponents = {
		std::type_index(typeid(Velocity)),
		std::type_index(typeid(Sprite))
	};
}

void FlipSpriteToXDirectionS::update(float dt)
{
	auto velPool = ComponentPoolManager::getInstance().getPool<Velocity>();
	auto spritePool = ComponentPoolManager::getInstance().getPool<Sprite>();

	for (auto& ID : entities)
	{
		auto& sprite = spritePool->get(ID);
		auto& velocity = velPool->get(ID);

		if (velocity.x < 0)
			sprite.flipHorizontally = true;
		else
			sprite.flipHorizontally = false;
	}
}
