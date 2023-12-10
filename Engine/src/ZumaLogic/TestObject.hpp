#pragma once

#include "../Managers/Entity.hpp"
#include "../Managers/Resource/ResourceManager.hpp"
#include "../Graphics/Animation.hpp"

#include "TestObject.hpp"

#include <vector>

class TestObject : public Entity, public Animation
{
public:
	TestObject(float x, float y, float size, Resource<Texture>* texture, int dimX, int dimY, int frameDuration);
	virtual void update();
	virtual void draw(RenderingAPI* renderer);
	Resource<Texture>* texture;
private:
	glm::vec2 pos;
	glm::vec2 size;
};