#include "RenderingS.hpp"

RenderingS::RenderingS(RenderingAPI* rAPI) : rAPI(rAPI)
{
	requiredComponents = { std::type_index(typeid(Transform)), std::type_index(typeid(Sprite)) };
}

void RenderingS::update(float dt)
{
	auto transformPool = ComponentPoolManager::getInstance().getPool<Transform>();
	auto spritePool = ComponentPoolManager::getInstance().getPool<Sprite>();
	auto animationPool = ComponentPoolManager::getInstance().getPool<Animation>();

	for (auto entID : animatedEntities)
	{
		Animation& anim = animationPool->get(entID);

		if (++anim.counter >= anim.frameDuration)
		{
			anim.counter = 0;
			if (!anim.frames.empty())
			{
				anim.currentFrame = (anim.currentFrame + 1) % anim.frames.size();
				spritePool->get(entID).textureSample = anim.frames[anim.currentFrame];
			}
		}
	}

	for (auto& layer : layers)
	{
		for (auto entID : layer.second)
		{
			Transform& trans = transformPool->get(entID);
			Sprite& sprite = spritePool->get(entID);

			rAPI->addSpriteInstance({ trans.x, trans.y }, { trans.width, trans.height }, trans.rot, sprite.getTexture()->getContents(), sprite.textureSample);
		}
	}

	/*for (auto entID : entities)
	{
		Transform& trans = transformPool->get(entID);
		Sprite& sprite = spritePool->get(entID);

		rAPI->addSpriteInstance({ trans.x, trans.y }, { trans.width, trans.height }, trans.rot, sprite.getTexture()->getContents(), sprite.textureSample);
	}*/
	rAPI->drawSpriteInstances();
}

bool RenderingS::addEntity(int ID, int layer)
{
	if (!SysBase::addEntity(ID))
		return false;
	
	if (ComponentPoolManager::getInstance().hasComponent<Animation>(ID))
		animatedEntities.insert(ID);

	layers[layer].insert(ID);

	return true;
}

bool RenderingS::addEntity(Entity ent, int layer)
{
	return RenderingS::addEntity(ent.getID(), layer);
}

void RenderingS::removeEntity(int ID)
{
	SysBase::removeEntity(ID);
	animatedEntities.erase(ID);
	for (auto& layer : layers)
		layer.second.erase(ID);
}
