#pragma once

#include <vector>
#include <set>

#include "../EntityManager.hpp"
#include "../ComponentPoolManager.hpp"

class SysBase
{
public:
	SysBase();
	bool addEntity(int ID);
	bool addEntity(Entity entity);
	void removeEntity(int ID);
	virtual void update(float dt) = 0;
protected:
	std::vector<std::type_index> requiredComponents;
	std::set<int> entities;
};