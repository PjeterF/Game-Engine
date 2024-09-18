#pragma once

#include "ComponentPoolManager.hpp"

#include <stack>
#include <set>
#include <unordered_set>
#include <nlohmann/json.hpp>

enum EntityTag
{
	DefaultTag=0,
	Barrier=1,
	PlayerCharacter=2,
	Enemy=3,
	FriendlyProjectile=4,
	EnemyProjectile=5,
};

class Entity
{
public:
	Entity(int ID);
	int getID();
	bool isValid();
	template<typename T>
	bool hasComponent();
	template<typename T>
	T& addComponent(T component);
	template<typename T>
	void removeComponent();
	template<typename T>
	T& getComponent();
	EntityTag getTag();
	void setTag(EntityTag tag);
	void kill();

	nlohmann::json serialize();
private:
	int ID;
	bool valid;
	ComponentPoolManager& manager;
};

class EntityManager
{
public:
	static EntityManager& getInstance();
	Entity createEntity(EntityTag tag = EntityTag::DefaultTag);
	Entity getEntity(int ID);
	bool entityExists(int ID);
	void deleteEntity(int ID);
	EntityTag getTag(int ID);
	void setTag(int ID, EntityTag tag);
	void update();
	void reset();
	const std::unordered_set<int>& getExistingEntities();
private:
	EntityManager();
	std::stack<int> availableID;
	std::unordered_set<int> existingEntities;
	std::vector<bool> alive;
	std::vector<EntityTag> tags;
	std::vector<int> entitiesToDelete;
};

template<typename T>
inline bool Entity::hasComponent()
{
	return manager.hasComponent<T>(this->ID);
}

template<typename T>
inline T& Entity::addComponent(T component)
{
	return manager.addComponent<T>(this->ID, component);
}

template<typename T>
inline void Entity::removeComponent()
{
	manager.removeComponent<T>(this->ID);
}

template<typename T>
inline T& Entity::getComponent()
{
	return manager.getComponent<T>(this->ID);
}