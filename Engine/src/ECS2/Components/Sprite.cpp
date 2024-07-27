#include "Sprite.hpp"

Sprite::Sprite(Resource<Texture>* textureRes, glm::vec4 textureSample, bool flipHorizontally) : textureSample(textureSample), flipHorizontally(flipHorizontally)
{
	if (textureRes == nullptr)
	{
		this->textureRes = nullptr;
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

nlohmann::json Sprite::serialize()
{
	nlohmann::json j;
	j["type"] = CBase::Sprite;

	j["texture"] = textureRes->getContents()->getFilepath();
	j["textureSample"] = { textureSample.x, textureSample.y , textureSample.z , textureSample.w };
	j["flipHorizontally"] = flipHorizontally;

	return j;
}

void Sprite::deSerialize(nlohmann::json j)
{
	textureRes = ResourceManager::getInstance().getResource<Texture>(j["texture"]);
	textureSample = { j["textureSample"][0], j["textureSample"][1] , j["textureSample"][2] , j["textureSample"][3] };
	flipHorizontally = j["flipHorizontally"];
}
