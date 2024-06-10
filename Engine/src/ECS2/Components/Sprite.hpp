#pragma once

#include "../../Managers/Resource/ResourceManager.hpp"
#include <glm/glm.hpp>

struct Sprite
{
	Sprite(Resource<Texture>* textureRes = nullptr, glm::vec4 textureSample = { 0, 0, 0, 0 });
	void setTexture(Resource<Texture>* textureRes);
	Resource<Texture>* getTexture();
	glm::vec4 textureSample;
private:
	Resource<Texture>* textureRes;
};