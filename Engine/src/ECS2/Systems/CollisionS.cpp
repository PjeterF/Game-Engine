#include "CollisionS.hpp"

#include <glm/glm.hpp>
#include <iostream>
#include <future>
#include <chrono>
#include <cmath>

#include "../Components/Velocity.hpp"

CollisionS::CollisionS(float cellSize) : cellSize(cellSize)
{
	requiredComponents = { std::type_index(typeid(Transform)), std::type_index(typeid(AABB)) };
}

void CollisionS::initialize(float cellSize)
{
	instanceImp(cellSize);
}

CollisionS& CollisionS::getInstance()
{
	return instanceImp(0);
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
		/*for (auto& col : collisions)
		{
			if (col.second.ID1 == *ID || col.second.ID2 == *ID)
			{
				collisionToRemove.push_back(col.first);
				break;
			}
		}*/
	}
	break;
	default:
		break;
	}
}

void CollisionS::update(float dt)
{
	for (auto& col : collisions)
	{
		if (col.second.state == CollisionS::Collision::State::exit)
		{
			collisionToRemove.push_back(col.first);
			continue;
		}

		Entity ent1(col.second.ID1);
		Transform& t1 = ent1.getComponent<Transform>();
		AABB& c1 = ent1.getComponent<AABB>();

		Entity ent2(col.second.ID2);
		Transform& t2 = ent2.getComponent<Transform>();
		AABB& c2 = ent2.getComponent<AABB>();

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

	/*std::vector<std::future<void>> futures;
	std::mutex colsMutex;*/

	for (auto& cell : grid)
	{
		for (int i = 0; i < cell.second.size(); i++)
		{
			Entity ent1(cell.second[i]);
			AABB& c1 = ent1.getComponent<AABB>();
			if (!c1.enabled)
				continue;
			Transform& t1 = ent1.getComponent<Transform>();

			for (int j = i + 1; j < cell.second.size(); j++)
			{
				Entity ent2(cell.second[j]);
				AABB& c2 = ent2.getComponent<AABB>();
				if (!c2.enabled)
					continue;
				Transform& t2 = ent2.getComponent<Transform>();

				int pairIndex;
				if (ent1.getID() < ent2.getID())
					pairIndex = utility::pairing::cantorPair(ent1.getID(), ent2.getID());
				else
					pairIndex = utility::pairing::cantorPair(ent2.getID(), ent1.getID());

				if (collisions.find(pairIndex) == collisions.end())
				{
					if (collided({ t1.x, t1.y }, { c1.width, c1.height }, { t2.x, t2.y }, { c2.width, c2.height }))
					{

						collisions[pairIndex] = Collision(ent1.getID(), ent2.getID());
					}
				}
			}
		}
		/*std::future<void> f = std::async(std::launch::async, [this, cell, &colsMutex]() {
			for (int i = 0; i < cell.second.size(); i++)
			{
				Entity ent1(cell.second[i]);
				Transform& t1 = ent1.getComponent<Transform>();
				AABB& c1 = ent1.getComponent<AABB>();

				if (!c1.enabled)
					continue;

				for (int j = i + 1; j < cell.second.size(); j++)
				{
					Entity ent2(cell.second[j]);
					Transform& t2 = ent2.getComponent<Transform>();
					AABB& c2 = ent2.getComponent<AABB>();

					if (!c2.enabled)
						continue;

					int pairIndex;
					if (ent1.getID() < ent2.getID())
						pairIndex = utility::pairing::cantorPair(ent1.getID(), ent2.getID());
					else
						pairIndex = utility::pairing::cantorPair(ent2.getID(), ent1.getID());

					std::lock_guard<std::mutex> lock(colsMutex);
					if (collisions.find(pairIndex) == collisions.end())
					{
						if (collided({ t1.x, t1.y }, { c1.width, c1.height }, { t2.x, t2.y }, { c2.width, c2.height }))
						{

							collisions[pairIndex] = Collision(ent1.getID(), ent2.getID());
						}
					}
				}
			}
			});
		futures.push_back(std::move(f));*/
	}

	/*for (auto& future : futures)
	{
		future.get();
	}*/
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

void CollisionS::updateResponse(float dt)
{
	for (auto& collision : collisions)
	{
		Entity ent1(collision.second.ID1);
		Entity ent2(collision.second.ID2);
		EntityTag tag1 = ent1.getTag();
		EntityTag tag2 = ent2.getTag();

		if ((tag1 | tag2) == (Barrier | DefaultTag) && collision.second.state == CollisionS::Collision::State::entry)
		{
			if (ent1.hasComponent<Velocity>())
			{
				Velocity& v = ent1.getComponent<Velocity>();
				v.x = -v.x;
				v.y = -v.y;
			}
			else
			{
				Velocity& v = ent2.getComponent<Velocity>();
				v.x = -v.x;
				v.y = -v.y;
			}
			continue;
		}
		if ((tag1 | tag2) == (Barrier | DefaultTag))
			continue;
		if (ent1.getTag() == EntityTag::DefaultTag && ent2.getTag() == EntityTag::DefaultTag && collision.second.state == CollisionS::Collision::State::entry)
		{
			Velocity& v1 = ent1.getComponent<Velocity>();
			Velocity& v2 = ent2.getComponent<Velocity>();

			Velocity v1Init = v1;
			Velocity v2Init = v2;

			v1.x = (2 * v2Init.x) / (2);
			v1.y = (2 * v2Init.y) / (2);

			v2.x = (2 * v1Init.x) / (2);
			v2.y = (2 * v1Init.y) / (2);
		}
		
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
			 AABB& c = ent.getComponent<AABB>();
			 Transform& t = ent.getComponent<Transform>();

			 if (pointCollision({ t.x, t.y }, { c.width, c.height }, point))
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

CollisionS& CollisionS::instanceImp(float cellSize)
{
	static CollisionS system(cellSize);
	return system;
}

void CollisionS::addToGrid(int ID)
{
	Entity ent = EntityManager::getInstance().getEntity(ID);
	Transform& trans = ent.getComponent<Transform>();
	AABB& col = ent.getComponent<AABB>();

	glm::ivec2 min = { (trans.x - col.width) / cellSize, (trans.y - col.height) / cellSize }, max = { (trans.x + col.width) / cellSize, (trans.y + col.height) / cellSize };

	for (int x = min.x; x <= max.x; x++)
	{
		for (int y = min.y; y <= max.y; y++)
		{
			grid[utility::pairing::integerPair(x, y)].push_back(ent.getID());
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
