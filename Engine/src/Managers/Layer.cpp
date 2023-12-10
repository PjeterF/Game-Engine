#include "Layer.hpp"
#include "EntityManager.hpp"

Layer::Layer(int priority)
{
	this->priority = priority;
}

void Layer::draw(RenderingAPI* renderer)
{
	for (auto entity : entities)
	{
		entity.second->draw(renderer);
	}
}

void Layer::update()
{
	for (auto entity : toBeAdded)
	{
		entities.insert({ entity->getID(), entity });
	}
	toBeAdded.clear();

	for (auto entity : entities)
	{
		entity.second->update();

		if (entity.second->pendingDelete)
			toBeRemoved.push_back(entity.second);
	}

	for (auto entity : toBeRemoved)
	{
		entities.erase(entity->getID());
		delete entity;
	}
	toBeRemoved.clear();
}

bool Layer::operator<(const Layer& layer)
{
	if (priority < layer.priority)
	{
		return true;
	}
	else
	{
		return false;
	}
}
