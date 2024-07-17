#include "RenderingS.hpp"  

#include<algorithm>

RenderingS::RenderingS(RenderingAPI* rAPI) : rAPI(rAPI)
{
	requiredComponents = {
		std::type_index(typeid(Transform)),
		std::type_index(typeid(Sprite)),
		std::type_index(typeid(Velocity))
	};
}

void RenderingS::update(float dt)
{
	auto transformPool = ComponentPoolManager::getInstance().getPool<Transform>();
	auto spritePool = ComponentPoolManager::getInstance().getPool<Sprite>();
	auto velocityPool = ComponentPoolManager::getInstance().getPool<Velocity>();

	for (auto& layer : layers)
	{
		for (auto entID : layer.second)
		{
			Transform& trans = transformPool->get(entID);
			Sprite& sprite = spritePool->get(entID);
			Velocity& vel = velocityPool->get(entID);

			if (vel.x < 0)
				sprite.flipHorizontally = true;
			else
				sprite.flipHorizontally = false;

			rAPI->addSpriteInstance({ trans.x, trans.y }, { trans.width, trans.height }, trans.rot, sprite.getTexture()->getContents(), sprite.textureSample, sprite.flipHorizontally);
		}	
	}

	rAPI->drawSpriteInstances();
}

bool RenderingS::addEntity(int ID)
{
	if (!SysBase::addEntity(ID))
		return false;

	layers[ComponentPoolManager::getInstance().getComponent<RenderingLayer>(ID).layer].push_back(ID);

	return true;
}

bool RenderingS::addEntity(Entity ent)
{
	return RenderingS::addEntity(ent.getID());
}

void RenderingS::removeEntity(int ID)
{
	SysBase::removeEntity(ID);

	Entity ent(ID);

	auto& layer = layers[ent.getComponent<RenderingLayer>().layer];
	auto it = std::find(layer.begin(), layer.end(), ID);
	if (it != layer.end())
		layer.erase(it);
}

struct
{
	bool operator()(int ID1, int ID2) const
	{
		static auto transformPool = ComponentPoolManager::getInstance().getPool<Transform>();

		return transformPool->get(ID1).y > transformPool->get(ID2).y;
	}
}yComparator;

void RenderingS::ySortLayer(int layer)
{
	auto it = layers.find(layer);
	if (it != layers.end())
		std::sort((*it).second.begin(), (*it).second.end(), yComparator);
}
