#include "CollisionGrid.hpp"

CollisionGrid::CollisionGrid(int x, int y, int dimX, int dimY, float cellSize)
{
	this->x = x;
	this->y = y;
	this->gridDimX = dimX;
	this->gridDimY = dimY;
	this->cellSize = cellSize;

	for (int i = 0; i < gridDimX * gridDimY + 1; i++)
	{
		grid.push_back(std::list<Collider*>());
	}
}

void CollisionGrid::drawGrid(RenderingAPI* renderer)
{
	for (int i = 0; i < gridDimY + 1; i++)
	{
		renderer->lineRenderer->draw(x, y + cellSize * i, x + gridDimX * cellSize, y + cellSize * i, 2, 0.6, 0.6, 0.6);
	}

	for (int i = 0; i < gridDimX + 1; i++)
	{
		renderer->lineRenderer->draw(x + cellSize * i, y, x + cellSize * i, y + gridDimY * cellSize, 5, 0.6, 0.6, 0.6);
	}

	for (int X = 0; X < gridDimX; X++)
	{
		for (int Y = 0; Y < gridDimY; Y++)
		{
			if (!grid[X + gridDimY * Y].empty())
			{
				renderer->lineRenderer->draw(x + X * cellSize, y + Y * cellSize, x + (X + 1) * cellSize, y + Y * cellSize, 5, 1, 0, 0);
				renderer->lineRenderer->draw(x + X * cellSize, y + (Y+1) * cellSize, x + (X + 1) * cellSize, y + (Y+1) * cellSize, 5, 1, 0, 0);

				renderer->lineRenderer->draw(x + X * cellSize, y + Y * cellSize, x + X * cellSize, y + (Y+1) * cellSize, 10, 1, 0, 0);
				renderer->lineRenderer->draw(x + (X+1) * cellSize, y + Y * cellSize, x + (X + 1) * cellSize, y + (Y + 1) * cellSize, 10, 1, 0, 0);
			}
		}
	}
}

void CollisionGrid::drawColliders(RenderingAPI* renderer)
{
	for (int x = 0; x < gridDimX; x++)
	{
		for (int y = 0; y < gridDimY; y++)
		{
			if (!grid[x + gridDimY * y].empty())
			{
				for (auto collider : grid[x + gridDimY * y])
				{
					collider->draw(renderer);
				}
			}
		}
	}
}

void CollisionGrid::insert(Collider* collider)
{
	auto t0 = getIndex(collider->colX - collider->colSizeX, collider->colY - collider->colSizeY);
	auto t1 = getIndex(collider->colX + collider->colSizeX, collider->colY + collider->colSizeY);

	int X1, Y1, X2, Y2;
	X1 = std::get<0>(t0);
	Y1 = std::get<1>(t0);
	X2 = std::get<0>(t1);
	Y2 = std::get<1>(t1);

	if (X1 < 0 || Y1 < 0 || X2 >= gridDimX || Y2 >= gridDimY)
	{
		grid[gridDimX * gridDimY].push_back(collider);
		colliders.push_back(collider);
		return;
	}

	for(int y = Y1; y <= Y2; y++)
	{
		for (int x = X1; x <= X2; x++)
		{
			grid[x + gridDimY * y].push_back(collider);
		}
	}
	colliders.push_back(collider);
}

void CollisionGrid::remove(Collider* collider)
{
	auto t0 = getIndex(collider->colX - collider->colSizeX, collider->colY - collider->colSizeY);
	auto t1 = getIndex(collider->colX + collider->colSizeX, collider->colY + collider->colSizeY);

	int X1, Y1, X2, Y2;
	X1 = std::get<0>(t0);
	Y1 = std::get<1>(t0);
	X2 = std::get<0>(t1);
	Y2 = std::get<1>(t1);

	if (X1 < 0 || Y1 < 0 || X2 >= gridDimX || Y2 >= gridDimY)
	{
		grid[gridDimX * gridDimY].remove(collider);
		colliders.remove(collider);
		return;
	}

	for (int y = Y1; y <= Y2; y++)
	{
		for (int x = X1; x <= X2; x++)
		{
			if (!grid[x + gridDimY * y].empty())
			{
				grid[x + gridDimY * y].remove(collider);
			}
		}
	}
	colliders.remove(collider);
}

void CollisionGrid::markAsDeleted(Collider* collider)
{
	for (auto collision : collisions)
	{
		if (collision->col1 == collider || collision->col2 == collider)
		{
			collision->state = Collision::DELETE;
		}
	}
}

// Should be called before physics update
void CollisionGrid::earlyUpdate()
{
	for (auto& cell : grid)
	{
		cell.clear();
	}
}

// Should be called after physics update
void CollisionGrid::lateUpdate()
{
	for (auto& collider : colliders)
	{
		this->insertToGrid(collider);
	}
}

void CollisionGrid::checkCollision()
{
	for (int x = 0; x < gridDimX; x++)
	{
		for (int y = 0; y < gridDimY; y++)
		{
			if (!grid[x + gridDimY * y].empty())
			{
				auto it = grid[x + gridDimY * y].begin();
				while (it!= grid[x + gridDimY * y].end())
				{
					auto it2 = it;
					it2++;
					while (it2 != grid[x + gridDimY * y].end())
					{
						if (!collisionExists((*it), (*it2)) && *it != *it2)
						{
							if ((*it)->checkCollision((*it2)))
							{
								collisions.push_back(new Collision(*it, *it2));
							}
						}
						it2++;
					}
					it++;
				}
			}
		}
	}
}

void CollisionGrid::handleCollisions()
{

	//std::cout << "Collisions: " << collisions.size() << std::endl;
	//std::cout << "colliders: " << colliders.size() << std::endl;

	auto it = collisions.begin();
	while (it != collisions.end())
	{
		if ((*it)->state == Collision::DELETE || (*it)->state == Collision::EXIT)
		{
			delete* it;
			it=collisions.erase(it);
		}
		else
		{
			(*it)->update();
		}
		if(it!=collisions.end())
			it++;
	}
}

void CollisionGrid::consoleShow()
{
	for (int x = 0; x < gridDimX; x++)
	{
		for (int y = 0; y < gridDimY; y++)
		{
			std::cout << grid[x + gridDimY * y].size() << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

void CollisionGrid::insertToGrid(Collider* collider)
{
	auto t0 = getIndex(collider->colX - collider->colSizeX, collider->colY - collider->colSizeY);
	auto t1 = getIndex(collider->colX + collider->colSizeX, collider->colY + collider->colSizeY);

	int X1, Y1, X2, Y2;
	X1 = std::get<0>(t0);
	Y1 = std::get<1>(t0);
	X2 = std::get<0>(t1);
	Y2 = std::get<1>(t1);

	if (X1 < 0 || Y1 < 0 || X2 >= gridDimX || Y2 >= gridDimY)
	{
		grid[gridDimX * gridDimY].push_back(collider);
		return;
	}

	for (int y = Y1; y <= Y2; y++)
	{
		for (int x = X1; x <= X2; x++)
		{
			grid[x + gridDimY * y].push_back(collider);
		}
	}
}

void CollisionGrid::removeFromGrid(Collider* collider)
{
	auto t0 = getIndex(collider->colX - collider->colSizeX, collider->colY - collider->colSizeY);
	auto t1 = getIndex(collider->colX + collider->colSizeX, collider->colY + collider->colSizeY);

	int X1, Y1, X2, Y2;
	X1 = std::get<0>(t0);
	Y1 = std::get<1>(t0);
	X2 = std::get<0>(t1);
	Y2 = std::get<1>(t1);

	if (X1 < 0 || Y1 < 0 || X2 >= gridDimX || Y2 >= gridDimY)
	{
		grid[gridDimX * gridDimY].remove(collider);
		return;
	}

	for (int y = Y1; y <= Y2; y++)
	{
		for (int x = X1; x <= X2; x++)
		{
			if (!grid[x + gridDimY * y].empty())
			{
				grid[x + gridDimY * y].remove(collider);
			}
		}
	}
}

bool CollisionGrid::collisionExists(Collider* col1, Collider* col2)
{
	for (auto collision : collisions)
	{
		if (col1 == collision->col1)
		{
			if (col2 == collision->col2)
			{
				return true;
			}
		}
		if (col1 == collision->col2)
		{
			if (col2 == collision->col1)
			{
				return true;
			}
		}
	}
	return false;
}

std::tuple<int, int> CollisionGrid::getIndex(float x, float y)
{
	int X = (int)(x - this->x) / cellSize;
	int Y = (int)(y - this->y) / cellSize;

	return { X, Y };
}

