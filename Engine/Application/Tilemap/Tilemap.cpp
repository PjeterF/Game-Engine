#include "Tilemap.hpp"

#include <nlohmann/json.hpp>
#include <fstream>

std::unordered_map<TileType, TileArchetype> TileArchetype::map = std::unordered_map<TileType, TileArchetype>();

void TileArchetype::initializeTileArchetypes()
{
	
}

Tilemap::Tilemap(float x, float y, std::vector<TileType> tiles) : tiles(tiles)
{
	this->position = { x, y };
}

void Tilemap::draw(RenderingAPI* API)
{
	for (auto tileType : tiles)
	{
		auto& archetype = TileArchetype::map[tileType];
		API->addSpriteInstance(
			position,
			{ position.x + TILE_SIZE, position.y + TILE_SIZE },
			0,
			ResourceManager::getInstance().getResource<Texture>(archetype.texturePath)->getContents(),
			archetype.division
		);
	}
	API->drawSpriteInstances();
}
