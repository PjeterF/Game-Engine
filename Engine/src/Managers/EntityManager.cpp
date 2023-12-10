#include "EntityManager.hpp"
#include <algorithm>
#include "../Events/EventManager.hpp"

EntityManager::EntityManager()
{
	addLayer(0);
	addLayer(1);
	addLayer(2);
	addLayer(3);
}

void EntityManager::addLayer(int priority)
{
	layers.push_back(new Layer(priority));
	/*if (layers.size() > 1)
	{
		std::sort(layers.begin(), layers.end());
	}*/
}

bool EntityManager::addEntity(Entity* entity, int layer)
{
	for (int i = 0; i < layers.size(); i++)
	{
		if (layers[i]->priority == layer)
		{
			layers[i]->toBeAdded.push_back(entity);
			return true;
		}
	}
	return false;
}

bool EntityManager::moveEntityToLayer(Entity* entity, int priority)
{
	if (entity == nullptr)
	{
		return false;
	}

	if (!layerExists(priority))
	{
		return false;
	}

	bool removed = true;
	for (auto layer : layers)
	{
		auto it = layer->entities.find(entity->getID());
		if (it != layer->entities.end())
		{
			layer->entities.erase(entity->getID());
			removed = true;
			break;
		}
	}
	if (!removed)
	{
		return false;
	}

	for (auto layer : layers)
	{
		if (layer->priority == priority)
		{
			layer->entities.insert({ entity->getID(), entity });
			return true;
		}
	}

	return false;
}

bool EntityManager::moveEntityToLayer(int ID, int priority)
{
	Entity* entity = getEntity(ID);
	if (entity == nullptr)
	{
		return false;
	}

	if (!layerExists(priority))
	{
		return false;
	}

	bool removed = true;
	for (auto layer : layers)
	{
		auto it = layer->entities.find(ID);
		if (it != layer->entities.end())
		{
			layer->entities.erase(ID);
			removed = true;
			break;
		}
	}
	if (!removed)
	{
		return false;
	}

	for (auto layer : layers)
	{
		if (layer->priority == priority)
		{
			entity->layer = priority;
			layer->entities.insert({ ID, entity });
			return true;
		}
	}

	return false;
}

bool EntityManager::removeEntity(int ID)
{
	for (auto layer : layers)
	{
		auto it = layer->entities.find(ID);
		if (it != layer->entities.end())
		{
			layer->toBeRemoved.push_back((*it).second);
			return true;
		}
	}
	return false;
}

Entity* EntityManager::getEntity(int ID)
{
	for (auto layer : layers)
	{
		auto it = layer->entities.find(ID);
		if (it != layer->entities.end())
		{
			return (*it).second;
		}
	}
	return nullptr;
}

bool EntityManager::layerExists(int layer)
{
	for (auto lay : layers)
	{
		if (lay->priority == layer)
		{
			return true;
		}
	}
	return false;
}

void EntityManager::deleteAll()
{
	for (auto layer : layers)
	{
		for (auto entity : layer->entities)
		{
			entity.second->destroy();
		}
	}
	update();
}

void EntityManager::draw(RenderingAPI* renderer)
{
	for (auto layer : layers)
	{
		layer->draw(renderer);
	}
}

void EntityManager::update()
{
	for (auto layer : layers)
	{
		layer->update();
	}
}
