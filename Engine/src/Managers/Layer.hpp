#pragma once

#include <map>
#include "../Managers/Entity.hpp"

class Layer
{
	friend class EntityManager;
	friend class Entity;
public:
	Layer(int pritority);

	void draw(RenderingAPI* renderer);
	void update();
	bool operator<(const Layer& layer);

	std::map<int, Entity*> entities;
	int priority;
private:
	std::vector<Entity*> toBeAdded;
	std::vector<Entity*> toBeRemoved;
};