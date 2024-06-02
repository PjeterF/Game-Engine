#pragma once

#include "../../Managers/Resource/ResourceManager.hpp"

struct Sprite
{
	Sprite(Resource<Texture>* textureRes = nullptr);
	void setTexture(Resource<Texture>* textureRes);
	Resource<Texture>* getTexture();
private:
	Resource<Texture>* textureRes;
};