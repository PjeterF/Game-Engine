#pragma once

#include "Resource/ResourceManager.hpp"

class Tileset
{
public:
	Tileset(Resource<Texture>* tilesetResourc);
private:
	Resource<Texture>* tilesetResource;
};