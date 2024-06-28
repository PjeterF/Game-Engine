#include "CollisionS.hpp"

#include <glm/glm.hpp>
#include <iostream>
#include <future>
#include <chrono>
#include <cmath>
#include <math.h>

#include "../Components/Velocity.hpp"

CollisionS::CollisionS(float cellSize) : cellSize(cellSize)
{
	requiredComponents = { std::type_index(typeid(Transform)), std::type_index(typeid(AABB)) };
}

void CollisionS::handleEvent(Event& event)
{
	SysBase::handleEvent(event);
	switch (event.getType())
	{
	case Event::EntityRemoval:
	{
		int* ID = (int*)event.getPayload();
		auto it = collisions.begin();
		while (it != collisions.end())
		{
			if ((*it).second.ID1 == *ID || (*it).second.ID2 == *ID)
			{
				collisions.erase(it);
				break;
			}
			it++;
		}
	}
	break;
	default:
		break;
	}
}

void CollisionS::update(float dt)
{
	auto transformPool = ComponentPoolManager::getInstance().getPool<Transform>();
	auto AABBPool = ComponentPoolManager::getInstance().getPool<AABB>();

	for (auto& col : collisions)
	{
		int ID1 = col.second.ID1;
		Transform& t1 = transformPool->get(ID1);
		AABB& c1 = AABBPool->get(ID1);

		int ID2 = col.second.ID2;
		Transform& t2 = transformPool->get(ID2);
		AABB& c2 = AABBPool->get(ID2);

		if (col.second.state == CollisionS::Collision::State::exit)
		{
			c1.collidingEntIDs.erase(ID2);
			c2.collidingEntIDs.erase(ID1);
			collisionToRemove.push_back(col.first);
			continue;
		}

		if (collided({ t1.x, t1.y }, { c1.width, c1.height }, { t2.x, t2.y }, { c2.width, c2.height }))
			col.second.state = CollisionS::Collision::State::stay;
		else
			col.second.state = CollisionS::Collision::State::exit;
	}

	for (auto& index : collisionToRemove)
	{
		collisions.erase(index);
	}
	collisionToRemove.clear();

	for (auto& cell : grid)
	{
		for (int i = 0; i < cell.second.size(); i++)
		{
			int ID1 = cell.second[i];
			AABB& c1 = AABBPool->get(ID1);
			if (!c1.enabled)
				continue;
			Transform& t1 = transformPool->get(ID1);

			for (int j = i + 1; j < cell.second.size(); j++)
			{
				int ID2 = cell.second[j];
				AABB& c2 = AABBPool->get(ID2);
				if (!c2.enabled)
					continue;
				Transform& t2 = transformPool->get(ID2);

				int pairIndex;
				if (ID1 < ID2)
					pairIndex = utility::pairing::cantorPair(ID1, ID2);
				else
					pairIndex = utility::pairing::cantorPair(ID2, ID1);

				if (collisions.find(pairIndex) == collisions.end())
				{
					if (collided({ t1.x, t1.y }, { c1.width, c1.height }, { t2.x, t2.y }, { c2.width, c2.height }))
					{
						c1.collidingEntIDs.insert(ID2);
						c2.collidingEntIDs.insert(ID1);
						collisions[pairIndex] = Collision(ID1, ID2);
					}
				}
			}
		}
	}
}

void CollisionS::lateUpdate(float dt)
{
	for (auto& cell : grid)
	{
		if (cell.second.empty())
			cellToRemove.push_back(cell.first);
		cell.second.clear();
	}
	for (auto index : cellToRemove)
	{
		grid.erase(index);
	}
	cellToRemove.clear();

	for (auto& entID : entities)
	{
		addToGrid(entID);
	}
}

std::vector<int> CollisionS::pointPick(glm::vec2 point)
{
	auto it = grid.find(utility::pairing::integerPair(point.x / cellSize, point.y / cellSize));
	if (it == grid.end())
		return std::vector<int>();
	else
	{
		std::vector<int> result;
		for (auto& ID : (*it).second)
		{
			 Entity ent = EntityManager::getInstance().getEntity(ID);
			 //AABB& c = ent.getComponent<AABB>();
			 Transform& t = ent.getComponent<Transform>();

			 if (pointCollision({ t.x, t.y }, { t.width, t.height }, point))
				 result.push_back(ID);
		}
		return result;
	}
}

bool CollisionS::addEntity(int ID)
{
	if (!SysBase::addEntity(ID))
		return false;

	addToGrid(ID);

	return true;
}

bool CollisionS::addEntity(Entity entity)
{
	return CollisionS::addEntity(entity.getID());
}

CollisionS::Collision& CollisionS::getCollision(int ID1, int ID2)
{
	int pairIndex;
	if (ID1 < ID2)
		pairIndex = utility::pairing::cantorPair(ID1, ID2);
	else
		pairIndex = utility::pairing::cantorPair(ID2, ID1);

	auto it = collisions.find(pairIndex);
	if (it == collisions.end())
		throw "Collision does not exist";
	else
		return (*it).second;
}

void CollisionS::addToGrid(int ID)
{
	static auto transformPool = ComponentPoolManager::getInstance().getPool<Transform>();
	static auto AABBPool = ComponentPoolManager::getInstance().getPool<AABB>();

	Transform& trans = transformPool->get(ID);
	AABB& col = AABBPool->get(ID);

	glm::ivec2 min = { (trans.x - col.width) / cellSize, (trans.y - col.height) / cellSize }, max = { (trans.x + col.width) / cellSize, (trans.y + col.height) / cellSize };

	for (int x = min.x; x <= max.x; x++)
	{
		for (int y = min.y; y <= max.y; y++)
		{
			grid[utility::pairing::integerPair(x, y)].push_back(ID);
		}
	}
}

bool CollisionS::collided(glm::vec2 pos1, glm::vec2 dim1, glm::vec2 pos2, glm::vec2 dim2)
{
	if (abs(pos1.x - pos2.x) > dim1.x + dim2.x)
		return false;
	if (abs(pos1.y - pos2.y) > dim1.y + dim2.y)
		return false;

	return true;
}

bool CollisionS::pointCollision(glm::vec2 colPos, glm::vec2 colDim, glm::vec2 point)
{
	if (abs(colPos.x - point.x) > colDim.x)
		return false;
	if (abs(colPos.y - point.y) > colDim.y)
		return false;

	return true;
}

CollisionS::Collision::Collision(int ID1, int ID2) :ID1(ID1), ID2(ID2)
{
}
