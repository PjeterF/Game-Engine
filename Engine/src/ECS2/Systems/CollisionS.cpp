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
	requiredComponents = {
		std::type_index(typeid(Transform)),
		std::type_index(typeid(AABB))
	};
}

void CollisionS::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::EntityRemoval:
	{
		int* ID = (int*)event.getPayload();
		
		std::vector<int> collisionsToRemove;
		auto it = collisions.begin();
		while (it != collisions.end())
		{
			if ((*it).second.ID1 == *ID || (*it).second.ID2 == *ID)
			{
				auto colPool = ComponentPoolManager::getInstance().getPool<AABB>();

				//if(colPool->has((*it).second.ID1))
				colPool->get((*it).second.ID1).collidingEntIDs.erase((*it).second.ID2);
				//if (colPool->has((*it).second.ID2))
				colPool->get((*it).second.ID2).collidingEntIDs.erase((*it).second.ID1);
				collisionsToRemove.push_back((*it).first);
			}
			it++;
		}
		for (auto collisionID : collisionsToRemove)
			collisions.erase(collisionID);
	}
	break;
	}
	SysBase::handleEvent(event);
}

void CollisionS::update(float dt)
{
	auto transformPool = ComponentPoolManager::getInstance().getPool<Transform>();
	auto AABBPool = ComponentPoolManager::getInstance().getPool<AABB>();

	for (auto& col : collisions)
	{
		int ID1 = col.second.ID1;
		/*if (!AABBPool->has(ID1))
			continue;
		if (!transformPool->has(ID1))
			continue;*/

		Transform& t1 = transformPool->get(ID1);
		AABB& c1 = AABBPool->get(ID1);

		int ID2 = col.second.ID2;
		/*if (!AABBPool->has(ID2))
			continue;
		if (!transformPool->has(ID2))
			continue;*/

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
			//if(!AABBPool->has(ID1))
			//	continue;
			//if (!transformPool->has(ID1))
			//	continue;

			AABB& c1 = AABBPool->get(ID1);
			if (!c1.enabled)
				continue;
			Transform& t1 = transformPool->get(ID1);

			for (int j = i + 1; j < cell.second.size(); j++)
			{
				int ID2 = cell.second[j];
				//if (!AABBPool->has(ID2))
				//	continue;
				//if (!transformPool->has(ID2))
				//	continue;

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

void CollisionS::drawGrid(RenderingAPI& rAPI, Camera& camera, float width, glm::vec4 color, bool commisionDrawing)
{
	float x = round(camera.getFrustrumX().x / cellSize) * cellSize;
	while (x < camera.getFrustrumX().y+cellSize)
	{
		rAPI.addQuadLineInstance({ x, camera.getFrustrumY().x }, { x, camera.getFrustrumY().y }, width, color);
		x += cellSize;
	}

	float y = round(camera.getFrustrumY().x / cellSize) * cellSize;
	while (y < camera.getFrustrumY().y + cellSize)
	{
		rAPI.addQuadLineInstance({ camera.getFrustrumX().x, y }, { camera.getFrustrumX().y, y }, width, color);
		y += cellSize;
	}

	if (commisionDrawing)
		rAPI.drawQuadInstances();
}

void CollisionS::drawCellsWithColliders(RenderingAPI& rAPI, Camera& camera, float width, glm::vec4 color, bool commisionDrawing)
{
	for (auto& cell : grid)
	{
		if (cell.second.empty())
			continue;

		glm::vec2 cellCoord = cellSize * (glm::vec2)utility::pairing::undoIntegerPair(cell.first);
		if (!camera.isPointInFrustrum(cellCoord, 10))
			continue;

		if (cellCoord.x < 0)
			cellCoord.x -= cellSize;
		if (cellCoord.y < 0)
			cellCoord.y -= cellSize;

		rAPI.addQuadLineInstance(cellCoord, cellCoord + cellSize, width, color);

		if(cellCoord.x == 0)
			rAPI.addQuadLineInstance(cellCoord - glm::vec2(cellSize, 0), cellCoord + cellSize - glm::vec2(cellSize, 0), width, color);
		if (cellCoord.y == 0)
			rAPI.addQuadLineInstance(cellCoord - glm::vec2(0, cellSize), cellCoord + cellSize - glm::vec2(0, cellSize), width, color);
	}

	if (commisionDrawing)
		rAPI.drawQuadInstances();
}

void CollisionS::addToGrid(int ID)
{
	static auto transformPool = ComponentPoolManager::getInstance().getPool<Transform>();
	static auto AABBPool = ComponentPoolManager::getInstance().getPool<AABB>();

	Transform& trans = transformPool->get(ID);
	AABB& col = AABBPool->get(ID);

	glm::ivec2 min = { (trans.x - col.width) / cellSize, (trans.y - col.height) / cellSize };
	glm::ivec2 max = { (trans.x + col.width) / cellSize, (trans.y + col.height) / cellSize };

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
