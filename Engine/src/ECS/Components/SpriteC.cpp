#include "SpriteC.hpp"

SpriteC::SpriteC(Resource<Texture>* texture) : ComponentBase(CType::Sprite)
{
	if(texture!=nullptr)
		texture->subscribe();
	this->texture = texture;
}

SpriteC::~SpriteC()
{
	texture->unsubscribe();
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
	if(this->texture!=nullptr)
		this->texture->unsubscribe();

	texture->subscribe();
	this->texture = texture;
}

Texture* SpriteC::getTexture()
{
	return texture->getContents();
}
