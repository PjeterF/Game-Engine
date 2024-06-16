#pragma once

#include <vector>
#include <set>

#include "../EntityManager.hpp"
#include "../../Events/EventManager.hpp"

class SysBase : public EventObserver
{
public:
	virtual void handleEvent(Event& event) override;
	virtual bool addEntity(int ID);
	virtual bool addEntity(Entity entity);
	virtual void removeEntity(int ID);
	virtual void update(float dt) = 0;
	virtual void lateUpdate(float dt);
protected:
	SysBase();
	std::vector<std::type_index> requiredComponents;
	std::set<int> entities;
};