#pragma once

#include <vector>
#include <set>
#include <cmath>

#include "../EntityManager.hpp"
#include "../../Events/EventManager.hpp"

class SysBase : public EventObserver
{
public:
	virtual ~SysBase();
	virtual void handleEvent(Event& event) override;
	virtual bool addEntity(int ID);
	virtual bool addEntity(Entity entity);
	virtual void removeEntity(int ID);
	virtual void update(float dt) = 0;
	virtual void lateUpdate(float dt);
	const std::unordered_set<int>& getEntitySet();

	bool entityIsIncluded(int ID);
protected:
	SysBase(bool deleteOnSceneEnd=false);
	std::vector<std::type_index> requiredComponents;
	std::unordered_set<int> entities;
	bool deleteOnSceneEnd = false;

	friend class Scene;
};