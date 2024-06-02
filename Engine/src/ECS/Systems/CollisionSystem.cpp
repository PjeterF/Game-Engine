#include "CollisionSystem.hpp"
#include "../Entity/EntManager.hpp"

CollisionSystem& CollisionSystem::instanceInplementation(int x, int y, int dimX, int dimY, float cellSize)
{
	static CollisionSystem system(x, y, dimX, dimY, cellSize);
	return system;
}

CollisionSystem::CollisionSystem(int x, int y, int dimX, int dimY, float cellSize) : SystemBase(UNPAUSED, true, CollisionSys)
{
	name = "ColisionSys(" + std::to_string(ID) + ")";

	requiredComponents = { BoxCollider, Transform_ };

	this->x = x;
	this->y = y;
	this->gridDimX = dimX;
	this->gridDimY = dimY;
	this->cellSize = cellSize;

	for (int i = 0; i < gridDimX * gridDimY + 1; i++)
	{
		grid.push_back(std::list<BoxColliderC*>());
	}
}

CollisionSystem& CollisionSystem::getInstance()
{
	return instanceInplementation();
}

void CollisionSystem::initialize(int x, int y, int dimX, int dimY, float cellSize)
{
	instanceInplementation(x, y, dimX, dimY, cellSize);
}

void CollisionSystem::reInitialize(int x, int y, int dimX, int dimY, float cellSize)
{
	CollisionSystem::getInstance().entities.clear();
	CollisionSystem::getInstance().grid.clear();

	for (auto& collision : CollisionSystem::getInstance().collisions)
	{
		delete collision.second;
	}
	CollisionSystem::getInstance().collisions.clear();

	CollisionSystem::getInstance().x = x;
	CollisionSystem::getInstance().y = y;
	CollisionSystem::getInstance().gridDimX = dimX;
	CollisionSystem::getInstance().gridDimY = dimY;
	CollisionSystem::getInstance().cellSize = cellSize;

	for (int i = 0; i < CollisionSystem::getInstance().gridDimX * CollisionSystem::getInstance().gridDimY + 1; i++)
	{
		CollisionSystem::getInstance().grid.push_back(std::list<BoxColliderC*>());
	}
}

void CollisionSystem::to_json(nlohmann::json& j) const
{
	j["type"] = type;

	j["cellSize"] = cellSize;
	j["x"] = x;
	j["y"] = y;
	j["gridDimX"] = gridDimX;
	j["gridDimY"] = gridDimY;

	j["entIDs"] = nlohmann::json::array();
	for (auto& ent : entities)
		j["entIDs"].push_back(ent->getID());
}

void CollisionSystem::from_json(nlohmann::json& j)
{
	CollisionSystem::getInstance().reInitialize(j["x"], j["y"], j["gridDimX"], j["gridDimY"], j["cellSize"]);

	for (auto& entID : j["entIDs"])
	{
		Ent* ent = EntManager::getInstance().getEntity(entID);
		if(ent!=nullptr)
			CollisionSystem::getInstance().addEntity(ent);
	}
}

void CollisionSystem::removeEntity(int ID)
{
	auto it = collisions.begin();
	while (it != collisions.end())
	{
		if ((*it).second->c1->ID == ID || (*it).second->c2->ID == ID)
		{
			delete (*it).second;
			it = collisions.erase(it);
		}
		else
		{
			it++;
		}
	}

	SystemBase::removeEntity(ID);
}

void CollisionSystem::update(float dt)
{
	
}

void CollisionSystem::lateUpdate(float dt)
{
	for (auto& entity : entities)
	{
		auto collider = (BoxColliderC*)entity->getComponent(CType::BoxCollider);
		auto transform = (TransformC*)entity->getComponent(CType::Transform_);

		collider->colX = transform->position.x;
		collider->colY = transform->position.y;
	}

	this->insertCollidersOfStoredEntities();
	this->checkCollisions();
}

void CollisionSystem::earlyUpdate(float dt)
{
	this->clearGrid();
}

void CollisionSystem::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::EntityRemoval:
	{
		auto entity = (Ent*)event.getPayload();
		auto collider = (BoxColliderC*)entity->getComponent(BoxCollider);
		if (collider == nullptr)
			return;

		// Delete collisions of this collider
		std::vector<CollisionECS*> collisionsToDelete;

		for (auto& ID : collider->collisionsIDs)
		{
			auto collision = (CollisionECS*)getCollision(ID);
			if (collision == nullptr)
				continue;
			collisionsToDelete.push_back(collision);
		}

		for (auto& collision : collisionsToDelete)
		{
			collisions.erase(collision->getID());
			delete collision;
		}

		// Remove entity form system
		SystemBase::removeEntity(entity->getID());
	}
	break;
	case Event::ComponentRemoval:
	{
		Ent* entity = (Ent*)event.getPayload();
		auto it = std::find(entities.begin(), entities.end(), entity);
		if (it == entities.end())
			break;

		if (!validateComponents(entity))
			removeEntity(entity->getID());
	}
	break;
	default:
		break;
	}
}

void CollisionSystem::checkCollisions()
{
	// Update collisions
	auto it = collisions.begin();
	while (it != collisions.end())
	{
		auto collision = (*it).second;

		if (collision->state == CollisionECS::State::Exit)
		{
			it++;
			collisions.erase(collision->getID());
			delete collision;
			continue;
		}

		if (checkCollision(collision->c1, collision->c2))
			collision->state = CollisionECS::State::Stay;
		else
			collision->state = CollisionECS::State::Exit;
		it++;
	}

	for (int x = 0; x < gridDimX; x++)
	{
		for (int y = 0; y < gridDimY; y++)
		{
			auto it = grid[x + gridDimY * y].begin();
			while (it != grid[x + gridDimY * y].end())
			{
				auto it2 = it;
				it2++;
				while (it2 != grid[x + gridDimY * y].end())
				{
					int collisionID;
					if((*it)->ID < (*it2)->ID)
						collisionID = utility::pairing::cantorPair((*it)->ID, (*it2)->ID);
					else
						collisionID = utility::pairing::cantorPair((*it2)->ID, (*it)->ID);

					auto collision = collisions.find(collisionID);
					if (collision == collisions.end())
					{
						// Add collision
						if (checkCollision(*it, *it2))
						{
							collisions.insert({ collisionID, new CollisionECS(*it, *it2) });
						}	
					}

					it2++;
				}
				it++;
			}
		}
	}
}

bool CollisionSystem::addEntity(Ent* entity)
{
	if (!SystemBase::addEntity(entity))
		return false;

	BoxColliderC* collider = (BoxColliderC*)entity->getComponent(BoxCollider);
	collider->ID = nextColID++;
	return true;
}

std::unordered_map<int, CollisionECS*>& CollisionSystem::getCollisions()
{
	return collisions;
}

CollisionECS* CollisionSystem::getCollision(int collisionID)
{
	auto it = collisions.find(collisionID);
	if (it == collisions.end())
		return nullptr;
	else
		return (*it).second;
}

CollisionECS* CollisionSystem::getCollision(int colliderID1, int colliderID2)
{
	if (colliderID1 < colliderID2)
		return getCollision(utility::pairing::cantorPair(colliderID1, colliderID2));
	else
		return getCollision(utility::pairing::cantorPair(colliderID2, colliderID1));
}

void CollisionSystem::drawGrid(RenderingAPI* API)
{
	for (int i = 0; i < gridDimY + 1; i++)
	{
		API->lineRenderer->draw(x, y + cellSize * i, x + gridDimX * cellSize, y + cellSize * i, 2, 0.6, 0.6, 0.6);
	}

	for (int i = 0; i < gridDimX + 1; i++)
	{
		API->lineRenderer->draw(x + cellSize * i, y, x + cellSize * i, y + gridDimY * cellSize, 5, 0.6, 0.6, 0.6);
	}

	for (int X = 0; X < gridDimX; X++)
	{
		for (int Y = 0; Y < gridDimY; Y++)
		{
			if (!grid[X + gridDimY * Y].empty())
			{
				API->lineRenderer->draw(x + X * cellSize, y + Y * cellSize, x + (X + 1) * cellSize, y + Y * cellSize, 5, 1, 0, 0);
				API->lineRenderer->draw(x + X * cellSize, y + (Y + 1) * cellSize, x + (X + 1) * cellSize, y + (Y + 1) * cellSize, 5, 1, 0, 0);

				API->lineRenderer->draw(x + X * cellSize, y + Y * cellSize, x + X * cellSize, y + (Y + 1) * cellSize, 10, 1, 0, 0);
				API->lineRenderer->draw(x + (X + 1) * cellSize, y + Y * cellSize, x + (X + 1) * cellSize, y + (Y + 1) * cellSize, 10, 1, 0, 0);
			}
		}
	}
}

void CollisionSystem::drawColliders(RenderingAPI* API)
{
	for (int x = 0; x < gridDimX; x++)
	{
		for (int y = 0; y < gridDimY; y++)
		{
			for (auto c : grid[x + gridDimY * y])
			{
				API->lineRenderer->draw(c->colX - c->colSizeX, c->colY - c->colSizeY, c->colX - c->colSizeX, c->colY + c->colSizeY, 4, 0.9, 0.5, 0);
				API->lineRenderer->draw(c->colX - c->colSizeX, c->colY + c->colSizeY, c->colX + c->colSizeX, c->colY + c->colSizeY, 2, 0.9, 0.5, 0);
				API->lineRenderer->draw(c->colX + c->colSizeX, c->colY + c->colSizeY, c->colX + c->colSizeX, c->colY - c->colSizeY, 4, 0.9, 0.5, 0);
				API->lineRenderer->draw(c->colX + c->colSizeX, c->colY - c->colSizeY, c->colX - c->colSizeX, c->colY - c->colSizeY, 2, 0.9, 0.5, 0);
			}
		}
	}
}

bool CollisionSystem::checkCollision(BoxColliderC* c1, BoxColliderC* c2)
{
	glm::vec2 delta = glm::vec2(c1->colX, c1->colY) - glm::vec2(c2->colX, c2->colY);

	float x = abs(delta.x) - (c1->colSizeX + c2->colSizeX);
	float y = abs(delta.y) - (c1->colSizeY + c2->colSizeY);

	if (x < 0 && y < 0)
		return true;
	else
		return false;
}

glm::ivec2 CollisionSystem::getIndex(float x, float y)
{
	int X = (int)(x - this->x) / cellSize;
	int Y = (int)(y - this->y) / cellSize;

	return { X, Y };
}

void CollisionSystem::insertToGrid(BoxColliderC* collider)
{
	auto t0 = getIndex(collider->colX - collider->colSizeX, collider->colY - collider->colSizeY);
	auto t1 = getIndex(collider->colX + collider->colSizeX, collider->colY + collider->colSizeY);

	int X1, Y1, X2, Y2;
	X1 = t0.x;
	Y1 = t0.y;
	X2 = t1.x;
	Y2 = t1.y;

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

void CollisionSystem::removeFromGrid(BoxColliderC* collider)
{
	auto t0 = getIndex(collider->colX - collider->colSizeX, collider->colY - collider->colSizeY);
	auto t1 = getIndex(collider->colX + collider->colSizeX, collider->colY + collider->colSizeY);

	int X1, Y1, X2, Y2;
	X1 = t0.x;
	Y1 = t0.y;
	X2 = t1.x;
	Y2 = t1.y;

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

void CollisionSystem::clearGrid()
{
	for (auto& cell : grid)
	{
		cell.clear();
	}
}

void CollisionSystem::insertCollidersOfStoredEntities()
{
	for (auto& entity : entities)
	{
		auto collider = (BoxColliderC*)entity->getComponent(CType::BoxCollider);
		this->insertToGrid(collider);
	}
}

CollisionECS::CollisionECS(BoxColliderC* c1, BoxColliderC* c2)
{
	this->c1 = c1;
	this->c2 = c2;

	if(c1->ID < c2->ID)
		this->ID = utility::pairing::cantorPair(c1->ID, c2->ID);
	else
		this->ID = utility::pairing::cantorPair(c2->ID, c1->ID);

	c1->collisionsIDs.push_back(ID);
	c2->collisionsIDs.push_back(ID);
}

CollisionECS::~CollisionECS()
{
	c1->collisionsIDs.remove(this->ID);
	c2->collisionsIDs.remove(this->ID);
}

int CollisionECS::getID()
{
	return ID;
}

BoxColliderC* CollisionECS::getOtherCollider(int thisColID)
{
	if (c1->ID == thisColID)
		return c2;
	else if (c2->ID == thisColID)
		return c1;
	else
		return nullptr;
}
