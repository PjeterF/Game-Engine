#include "RenderingS.hpp"  

#include<algorithm>

RenderingS::RenderingS(RenderingAPI* rAPI) : rAPI(rAPI)
{
	requiredComponents = {
		std::type_index(typeid(RenderingLayer))
	};
}

void RenderingS::update(float dt)
{
	auto transformPool = ComponentPoolManager::getInstance().getPool<Transform>();
	auto emitterPool = ComponentPoolManager::getInstance().getPool<Emitter>();
	auto spritePool = ComponentPoolManager::getInstance().getPool<Sprite>();
	auto velocityPool = ComponentPoolManager::getInstance().getPool<Velocity>();

	for (auto& layer : layers)
	{
		for (auto entID : layer.second)
		{
			if (ComponentPoolManager::getInstance().hasComponent<Sprite>(entID) && ComponentPoolManager::getInstance().hasComponent<Transform>(entID))
			{
				Transform& trans = transformPool->get(entID);
				Sprite& sprite = spritePool->get(entID);

				if (sprite.getTexture() != nullptr)
					rAPI->addSpriteInstance({ trans.x, trans.y }, { trans.width, trans.height }, trans.rot, sprite.getTexture()->getContents(), sprite.textureSample, sprite.flipHorizontally);
			}
			if (ComponentPoolManager::getInstance().hasComponent<Emitter>(entID))
			{
				Emitter& emitter = emitterPool->get(entID);

				emitter.emitter.draw(rAPI);
			}
		}	

		rAPI->drawSpriteInstances();
		rAPI->drawQuadInstances();
	}
}

bool RenderingS::addEntity(int ID)
{
	if (!SysBase::addEntity(ID))
		return false;

	Entity ent(ID);

	RenderingLayer& layer = ent.getComponent<RenderingLayer>();
	layers[layer.layer].push_back(ID);

	return true;
}

bool RenderingS::addEntity(Entity ent)
{
	return RenderingS::addEntity(ent.getID());
}

void RenderingS::removeEntity(int ID)
{
	if (entities.find(ID) != entities.end())
	{
		RenderingLayer& layer = ComponentPoolManager::getInstance().getComponent<RenderingLayer>(ID);
		layers[layer.layer].erase(std::find(layers[layer.layer].begin(), layers[layer.layer].end(), ID));
	}

	SysBase::removeEntity(ID);
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

bool RenderingS::moveToLayer(int ID, int newLayer)
{
	if (entities.find(ID) == entities.end())
		return false;

	auto& layer = ComponentPoolManager::getInstance().getComponent<RenderingLayer>(ID);
	layers[layer.layer].erase(std::find(layers[layer.layer].begin(), layers[layer.layer].end(), ID));

	layers[newLayer].push_back(ID);
	layer.layer = newLayer;

	return true;
}
