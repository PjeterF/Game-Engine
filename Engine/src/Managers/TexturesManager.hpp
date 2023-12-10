#pragma once
#include <map>
#include <string>

#include "../OpenGL/Texture.hpp"

struct Content
{
	std::string path;
	Texture* texture;
};

class TexturesManager
{
public:
	TexturesManager() {};

	static TexturesManager& getInstance()
	{
		static TexturesManager instance;
		return instance;
	}
	bool createTexture(std::string id, std::string texturePath, int spriteSheetCols=1, int spriteSheetRows=1);
	bool deleteTexture(std::string id);
	Texture* getTexture(std::string id);
	std::map<std::string, Content>::iterator begin();
	std::map<std::string, Content>::iterator end();
private:
	std::map<std::string, Content> textures;
};

