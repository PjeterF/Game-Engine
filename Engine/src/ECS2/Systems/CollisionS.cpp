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
		//switch (col.second.state)
		//{
		//	case CollisionS::Collision::State::entry:
		//		std::cout << col.second.ID1 << "|" << col.second.ID2 << " " << "entry\n";
		//		break;
		//	case CollisionS::Collision::State::stay:
		//		std::cout << col.second.ID1 << "|" << col.second.ID2 << " " << "stay\n";
		//		break;
		//	case CollisionS::Collision::State::exit:
		//		std::cout << col.second.ID1 << "|" << col.second.ID2 << " " << "exit\n";
		//		break;
		//}

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
			grid[utility::pairing::integerPair(x, y)].insert(ent.getID());
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
