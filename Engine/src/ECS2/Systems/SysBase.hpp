#pragma once

#include <vector>
#include <set>

#include "../EntityManager.hpp"

class SysBase
{
public:
	SysBase();
	virtual bool addEntity(int ID);
	virtual bool addEntity(Entity entity);
	virtual void removeEntity(int ID);
	virtual void update(float dt) = 0;
	virtual void lateUpdate(float dt);
protected:
	std::vector<std::type_index> requiredComponents;
	std::set<int> entities;
};