#include "Collider.hpp"
#include "CollisionGrid.hpp"

Collider::Collider(float x, float y, float size)
{
	CollisionGrid::getInstance().insert(this);
	ID = generateID();
	colX = x;
	colY = y;
	colSizeX = size;
	colSizeY = size;
	collision = true;
}

Collider::Collider(float x, float y, float sizeX, float sizeY)
{
	CollisionGrid::getInstance().insert(this);
	ID = generateID();
	colX = x;
	colY = y;
	colSizeX = sizeX;
	colSizeY = sizeY;
	collision = true;
}

Collider::~Collider()
{
	CollisionGrid::getInstance().markAsDeleted(this);
	CollisionGrid::getInstance().remove(this);
}

bool Collider::checkCollision(Collider* other)
{
	if (this->collision == false || other->collision == false)
	{
		return false;
	}

	glm::vec2 delta = glm::vec2(colX,colY) - glm::vec2(other->colX, other->colY);

	float x = abs(delta.x) - (colSizeX + other->colSizeX);
	float y = abs(delta.y) - (colSizeY + other->colSizeY);

	if (x < 0 && y < 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Collider::checkPointCollision(float x, float y)
{
	bool xAxis = false;
	bool yAxis = false;

	if (x<colX + colSizeX && x>colX - colSizeX)
	{
		xAxis = true;
	}
	if (y<colY + colSizeY && y>colY - colSizeY)
	{
		yAxis = true;
	}

	if (xAxis && yAxis)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Collider::onEntry(Collider* other)
{
	
}

void Collider::onStay(Collider* other)
{
	
}

void Collider::onExit(Collider* other)
{
	
}

void Collider::updateCollider(float xPos, float yPos, float size)
{
	colX = xPos;
	colY = yPos;
	colSizeX = size;
	colSizeY = size;
}

void Collider::updateCollider(float xPos, float yPos, float sizeX, float sizeY)
{
	colX = xPos;
	colY = yPos;
	colSizeX = sizeX;
	colSizeY = sizeY;
}

void Collider::draw(RenderingAPI* renderer)
{
	renderer->lineRenderer->draw(colX - colSizeX, colY - colSizeY, colX - colSizeX, colY + colSizeY, 4, 0.9, 0.5, 0);
	renderer->lineRenderer->draw(colX - colSizeX, colY + colSizeY, colX + colSizeX, colY + colSizeY, 2, 0.9, 0.5, 0);
	renderer->lineRenderer->draw(colX + colSizeX, colY + colSizeY, colX + colSizeX, colY - colSizeY, 4, 0.9, 0.5, 0);
	renderer->lineRenderer->draw(colX + colSizeX, colY - colSizeY, colX - colSizeX, colY - colSizeY, 2, 0.9, 0.5, 0);
}

int Collider::getID()
{
	return ID;
}

int Collider::generateID()
{
	static int ID = 0;
	return ID++;
}
