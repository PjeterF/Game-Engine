#pragma once

#include "Collider.hpp"
#include "../../Graphics/GeneralRenderer.hpp"
#include "Collision.hpp"

#include <vector>
#include <list>
#include <tuple>
#include <unordered_map>

class CollisionGrid
{
public:
	CollisionGrid(int x, int y, int dimX, int dimY, float cellSize);
	static void initialize(int x, int y, int dimX, int dimY, float cellSize)
	{
		instanceImp(x, y, dimX, dimY, cellSize);
	}
	static CollisionGrid& getInstance()
	{
		return instanceImp();
	}
	void drawGrid(RenderingAPI* renderer);
	void drawColliders(RenderingAPI* renderer);
	void insert(Collider* collider);
	void remove(Collider* collider);
	void markAsDeleted(Collider* collider);
	void earlyUpdate();
	void lateUpdate();
	void checkCollision();
	void handleCollisions();

	void consoleShow();
private:
	static CollisionGrid& instanceImp(int x=0, int y=0, int dimX=0, int dimY=0, float cellSize=0)
	{
		static CollisionGrid grid(x, y, dimX, dimY, cellSize);
		return grid;
	}
	void insertToGrid(Collider* collider);
	void removeFromGrid(Collider* collider);
	bool collisionExists(Collider* col1, Collider* col2);
	std::tuple<int, int> getIndex(float x, float y);
	std::vector<std::list<Collider*>> grid;
	std::list<Collider*> colliders;
	std::list<Collision*> collisions;
	float cellSize;
	int gridDimX, gridDimY;
	float x, y;
};