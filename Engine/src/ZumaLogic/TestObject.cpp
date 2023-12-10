#include "TestObject.hpp"

TestObject::TestObject(float x, float y, float size, Resource<Texture>* texture, int dimX, int dimY, int frameDuration) : Animation(texture, dimX, dimY, frameDuration)
{
	pos = glm::vec2(x, y);
	this->size = glm::vec2(size, size);
	this->texture = texture;
}

void TestObject::update()
{
	pos.x = pos.x - 1;

	Animation::update();
}

void TestObject::draw(RenderingAPI* renderer)
{
	if (this->texture == nullptr)
		return;
	if (this->texture->getContents() == nullptr)
		return;
	
	renderer->spriteRenderer->draw(pos, size, 0, texture->getContents(), getCurrFrame());
}
