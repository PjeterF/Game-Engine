#pragma once

#include "Ent.hpp"
#include "../../src/Events/EventManager.hpp"

class EntManager
{
public:
	static EntManager& getInstance();
	void update();
	Ent* createEntity(ETag tag = Default);
	Ent* getEntity(int ID);
	std::unordered_map<int, Ent*>::iterator begin();
	std::unordered_map<int, Ent*>::iterator end();
private:
	EntManager();
	int nextID = 0;
	std::unordered_map<int, Ent*> entities;
	std::vector<Ent*> toBeAdded;
	std::vector<Ent*> toBeRemoved;

	friend class SceneMenu;
	friend class EntityPropertiesMenu;
};