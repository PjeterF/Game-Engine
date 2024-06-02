#pragma once

#include "Entity.hpp"

#include <stack>
#include <set>

#define MAX_ENTITIES 10000

class EntityManager
{
public:
	static EntityManager& getInstance();
	Entity createEntity();
	Entity getEntity(int ID);
	bool entityExists(int ID);
	void deleteEntity(int ID);
private:
	EntityManager();
	std::stack<int> availableID;
	std::vector<bool> alive;
};