#pragma once

#include "../../Managers/Resource/ResourceManager.hpp"
#include <glm/glm.hpp>

#include "CBase.hpp"

struct Sprite : public CBase
{
	Sprite(Resource<Texture>* textureRes = nullptr, glm::vec4 textureSample = { 0, 0, 0, 0 }, bool flipHorizontally=false);
	void setTexture(Resource<Texture>* textureRes);
	Resource<Texture>* getTexture();
	glm::vec4 textureSample;
	bool flipHorizontally;

	virtual nlohmann::json serialize() override;
	virtual void deSerialize(nlohmann::json j) override;
private:
	Resource<Texture>* textureRes;
};