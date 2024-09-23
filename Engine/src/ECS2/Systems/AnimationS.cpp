#include "AnimationS.hpp"

AnimationS::AnimationS()
{
	requiredComponents = {
		std::type_index(typeid(Sprite)),
		std::type_index(typeid(Animation)),
		std::type_index(typeid(Velocity)),
	};
}

void AnimationS::update(float dt)
{
	auto spritePool = ComponentPoolManager::getInstance().getPool<Sprite>();
	auto animationPool = ComponentPoolManager::getInstance().getPool<Animation>();
	auto velocityPool = ComponentPoolManager::getInstance().getPool<Velocity>();

	for (auto entID : entities)
	{
		Animation& anim = animationPool->get(entID);
		Velocity& vel = velocityPool->get(entID);

		if (glm::length(glm::vec2(vel.x, vel.y)) < 0.001)
			anim.state = Animation::idle;
		else
			anim.state = Animation::run;

		anim.counter = anim.counter + dt;
		if (++anim.counter >= anim.frameDuration)
		{
			anim.counter = 0;

			auto it = anim.framesMap.find(anim.state);
			if (it != anim.framesMap.end())
			{
				const auto& frames = (*it).second;
				if (!frames.empty())
				{
					anim.currentFrame = (anim.currentFrame + 1) % frames.size();
					spritePool->get(entID).textureSample = frames[anim.currentFrame];
				}
			}
		}
	}
}
