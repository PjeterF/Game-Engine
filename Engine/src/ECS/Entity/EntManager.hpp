#pragma once

#include "Ent.hpp"
#include "../../src/Events/EventManager.hpp"

class SystemsManager;

class EntManager
{
public:
	static EntManager& getInstance();
	void update();
	Ent* createEntity(ETag tag = Default);
	Ent* getEntity(int ID);
	bool addEntity(Ent* entity);
	std::unordered_map<int, Ent*>::iterator begin();
	std::unordered_map<int, Ent*>::iterator end();

	int getNumberOfCreatedEntities();
	void deleteAllEntities();
private:
	EntManager();
	int nextID = 0;
	std::unordered_map<int, Ent*> entities;
	std::vector<Ent*> toBeAdded;
	std::vector<Ent*> toBeRemoved;

	friend class SceneMenu;
	friend class EntityPropertiesMenu;
	friend bool serializeScene(std::string filepath, EntManager& entityManager, SystemsManager& systemsManager);
	friend bool deSerializeScene(std::string filepath, EntManager& entityManager, SystemsManager& systemsManager);
};