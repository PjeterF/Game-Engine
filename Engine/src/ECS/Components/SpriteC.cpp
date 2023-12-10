#include "SpriteC.hpp"

SpriteC::SpriteC(Resource<Texture>* texture) : ComponentBase(CType::Sprite)
{
	texture->subscribe();
	this->texture = texture;
}

SpriteC::~SpriteC()
{
	texture->unsubscribe();
}

nlohmann::json SpriteC::serialize()
{
	nlohmann::json j = ComponentBase::serialize();

	j["filepath"] = this->getTexture()->getFilepath();

	return j;
}

void SpriteC::to_json(nlohmann::json& j) const
{
	ComponentBase::to_json(j);
	j["filepath"] = this->texture->getContents()->getFilepath();
}

void SpriteC::from_json(nlohmann::json& j)
{
	this->setTexture(ResourceManager::getInstance().getResource<Texture>(j["filepath"]));
}

void SpriteC::setTexture(Resource<Texture>* texture)
{
	this->texture->unsubscribe();

	texture->subscribe();
	this->texture = texture;
}

Texture* SpriteC::getTexture()
{
	return texture->getContents();
}
