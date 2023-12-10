#include "Sprite.hpp"
#include "../Managers/TexturesManager.hpp"

Sprite::Sprite(float x, float y, float size, Resource<Texture>* texture, bool collision, float angle, int layer) : Collider(x,y,size), Entity(layer)
{
	type = sprite;
	this->position = glm::vec2(x, y);
	this->size = glm::vec2(size, size);
	this->texture = texture;
	texture->subscribe();
	this->angle = angle;
	this->collision = collision;
}

Sprite::Sprite(float x, float y, float sizeX, float sizeY, Resource<Texture>* texture, bool collision, float angle, int layer) : Collider(x, y, sizeX), Entity(layer)
{
	type = sprite;
	this->position = glm::vec2(x, y);
	this->size = glm::vec2(sizeX, sizeY);
	this->texture = texture;
	texture->subscribe();
	this->angle = angle;
	this->collision = collision;
}

Sprite::~Sprite()
{
	texture->unsubscribe();
}

void Sprite::draw(RenderingAPI* renderer)
{
	if (texture != nullptr)
	{
		renderer->spriteRenderer->draw(position, size, angle, texture->getContents());
	}
}

void Sprite::update()
{
	if (direction.x!=0 || direction.y != 0)
	{
		position = position + velocity * glm::normalize(direction);
	}
	updateCollider(position.x, position.y, size.x, size.y);
}

nlohmann::json Sprite::serialize()
{
	using json = nlohmann::json;

	json j;

	j["position"] = { position.x, position.y };
	j["size"] = { size.x, size.y };
	j["angle"] = angle;
	j["velocity"] = velocity;
	j["collision"] = collision;
	j["texture"] = texture->getContents()->getFilepath();
	j["direction"] = { direction.x, direction.y };

	return j;

}

Sprite* Sprite::deSerialize(nlohmann::json j)
{
	glm::vec2 position = glm::vec2(j["position"][0], j["position"][1]);
	glm::vec2 size = glm::vec2(j["size"][0], j["size"][1]);
	glm::vec2 direction = glm::vec2(j["direction"][0], j["direction"][1]);
	float angle = j["angle"];
	float velocity = j["velocity"];
	bool collision = j["collision"];
	//if (ResourceManager::getInstance().getResource<Texture>(j["texture"]) == nullptr)
	//	ResourceManager::getInstance().createResourceFromFile<Texture>(j["texture"]);

	Sprite* sprite = new Sprite(position.x, position.y, size.x, size.y, ResourceManager::getInstance().getResource<Texture>(j["texture"]), collision, angle);
	sprite->direction = direction;
	sprite->velocity = velocity;

	return sprite;
}
