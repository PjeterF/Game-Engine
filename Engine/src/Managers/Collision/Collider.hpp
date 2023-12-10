#pragma once

#include "glm/vec2.hpp"
#include "../../Graphics/GeneralRenderer.hpp"

class Collider
{
public:
	Collider(float x, float y, float size);
	Collider(float x, float y, float sizeX, float sizeY);
	virtual ~Collider();
	bool checkCollision(Collider* other);
	bool checkPointCollision(float x, float y);
	virtual void onEntry(Collider* other);
	virtual void onStay(Collider* other);
	virtual void onExit(Collider* other);
	void updateCollider(float xPos, float yPos, float size);
	void updateCollider(float xPos, float yPos, float sizeX, float sizeY);
	void draw(RenderingAPI* renderer);
	int getID();

	float colX, colY, colSizeX, colSizeY;
	bool collision=true;
private:
	int ID;
	int generateID();
};