#pragma once

#include"../Entity/Ent.hpp"
#include <unordered_map>
#include <string>
#include "../../Events/EventManager.hpp"

enum SystemSet
{
	PAUSED,
	UNPAUSED
};

class SystemBase : public EventObserver
{
public:
	SystemBase(SystemSet set = UNPAUSED);
	~SystemBase();

	virtual void earlyUpdate(float dt);
	virtual void update(float dt) = 0;
	virtual void lateUpdate(float dt);

	virtual bool addEntity(Ent* entity);
	virtual void removeEntity(int ID);
	virtual void handleEvent(Event& event) override;

	int getID();
	std::string getName();
protected:
	bool validateComponents(Ent* entity);

	std::string name;
	static int nextID;
	int ID;
	std::unordered_map<int, Ent*> entities;
	std::vector<CType> requiredComponents;
};