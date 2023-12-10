#pragma once

#include "../Managers/Entity.hpp"
#include "Layer.hpp"

#include <list>
#include <vector>

class EntityManager
{
public:
	EntityManager();
	static EntityManager& getInstance()
	{
		static EntityManager instance;
		return instance;
	}
	void addLayer(int priority);
	bool addEntity(Entity* entity, int layer=0);
	bool moveEntityToLayer(Entity* entity, int layer);
	bool moveEntityToLayer(int ID, int priority);
	bool removeEntity(int ID);
	Entity* getEntity(int ID);
	bool layerExists(int layer);
	void deleteAll();
	void draw(RenderingAPI* renderer);
	void update();

	std::vector<Layer*> layers;
};