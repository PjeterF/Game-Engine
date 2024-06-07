#include "CollisionS.hpp"

#include <glm/glm.hpp>
#include <iostream>

CollisionS::CollisionS(float x, float y, float cellSize) : x(x), y(y), cellSize(cellSize)
{
	requiredComponents = { std::type_index(typeid(Transform)), std::type_index(typeid(AABB)) };
}

void CollisionS::initialize(float x, float y, float cellSize)
{
	instanceImp(x, y, cellSize);
}

CollisionS& CollisionS::getInstance()
{
	return instanceImp(0, 0, 0);
}

void CollisionS::update(float dt)
{
	std::cout << grid.size() << "\n";
	collisions.clear();
	for (auto& cell : grid)
	{
		for (auto& entId : cell.second)
		{
			auto it = cell.second.begin();
			while (it != cell.second.end())
			{
				Entity ent1(*it);
				Transform& t1 = ent1.getComponent<Transform>();
				AABB& c1 = ent1.getComponent<AABB>();

				auto it2 = it;
				it2++;
				while (it2 != cell.second.end())
				{
					Entity ent2(*it2);
					Transform& t2 = ent2.getComponent<Transform>();
					AABB& c2 = ent2.getComponent<AABB>();

					if (collided({ t1.x, t1.y }, { c1.width, c1.height }, { t2.x, t2.y }, { c2.width, c2.height }))
					{
						collisions[ent1.getID()].insert(ent2.getID());
						collisions[ent2.getID()].insert(ent1.getID());
						std::cout << "Collision: " << ent1.getID() << " | " << ent2.getID() << "\n";
					}

					it2++;
				}
				it++;
			}
		}
	}
}

void CollisionS::lateUpdate(float dt)
{
	grid.clear();
	for (auto& entID : entities)
	{
		addToGrid(entID);
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

CollisionS& CollisionS::instanceImp(float x, float y, float cellSize)
{
	static CollisionS system(x, y, cellSize);
	return system;
}

void CollisionS::addToGrid(int ID)
{
	Entity ent = EntityManager::getInstance().getEntity(ID);
	Transform& trans = ent.getComponent<Transform>();
	AABB& col = ent.getComponent<AABB>();

	glm::ivec2 min = { trans.x - col.width, trans.y - col.height }, max = { trans.x + col.width, trans.y + col.height };

	for (int x = min.x; x <= max.x; x++)
	{
		for (int y = min.y; y <= max.y; y++)
		{
			grid[utility::pairing::integerPair(x / cellSize, y / cellSize)].insert(ent.getID());
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

CollisionS::Collision::Collision(int otherID) :otherID(otherID)
{
}
