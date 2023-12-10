#include "TexturesManager.hpp"

bool TexturesManager::createTexture(std::string id, std::string texturePath, int spriteSheetCols, int spriteSheetRows)
{
	auto it = textures.find(id);
	if (it == textures.end())
	{
		Content cont;
		cont.path = texturePath;
		cont.texture = new Texture(texturePath, true);
		textures.insert({ id, cont });
		return true;
	}
	else
	{
		return false;
	}
}

bool TexturesManager::deleteTexture(std::string id)
{
	auto it = textures.find(id);
	if (it != textures.end())
	{
		delete (*it).second.texture;
		textures.erase(it);
		return true;
	}
	else
	{
		return false;
	}
}

Texture* TexturesManager::getTexture(std::string id)
{
	auto it = textures.find(id);
	if (it != textures.end())
	{
		return (*it).second.texture;
	}
	else
	{
		return nullptr;
	}
}

std::map<std::string, Content>::iterator TexturesManager::begin()
{
	return textures.begin();
}

std::map<std::string, Content>::iterator TexturesManager::end()
{
	return textures.end();
}
