#include "Sprite.hpp"

Sprite::Sprite(Resource<Texture>* textureRes, glm::vec4 textureSample, bool flipHorizontally) : textureSample(textureSample), flipHorizontally(flipHorizontally)
{
	if (textureRes == nullptr)
	{
		this->textureRes = ResourceManager::getInstance().getResource<Texture>("default");
	}
	else
	{
		textureRes->subscribe();
		this->textureRes = textureRes;
	}
}

void Sprite::setTexture(Resource<Texture>* textureRes)
{
	if (this->textureRes != nullptr)
		this->textureRes->unsubscribe();

	textureRes->subscribe();
	this->textureRes = textureRes;
}

Resource<Texture>* Sprite::getTexture()
{
	return textureRes;
}