#include "CollisionS.hpp"

#include <glm/glm.hpp>
#include <iostream>

#include "../Components/Velocity.hpp"

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

	for (auto& cell : grid)
	{
		for (int i = 0; i < cell.second.size(); i++)
		{
			Entity ent1(cell.second[i]);
			Transform& t1 = ent1.getComponent<Transform>();
			AABB& c1 = ent1.getComponent<AABB>();

			for (int j = i + 1; j < cell.second.size(); j++)
			{
				Entity ent2(cell.second[j]);
				Transform& t2 = ent2.getComponent<Transform>();
				AABB& c2 = ent2.getComponent<AABB>();

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
	}
}

void CollisionS::lateUpdate(float dt)
{
	for (auto& cell : grid)
	{
		cell.second.clear();
	}
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

		if (ent1.getTag() == EntityTag::DefaultTag && ent2.getTag() == EntityTag::DefaultTag)
		{
			if (collision.second.state == CollisionS::Collision::State::entry)
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

CollisionS::Collision::Collision(int ID1, int ID2) :ID1(ID1), ID2(ID2)
{
}
