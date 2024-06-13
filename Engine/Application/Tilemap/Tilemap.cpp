#include "Tilemap.hpp"

#include <nlohmann/json.hpp>
#include <fstream>

std::unordered_map<TileType, TileArchetype> TileArchetype::map = std::unordered_map<TileType, TileArchetype>();

void TileArchetype::initializeTileArchetypes()
{
	
}

Tilemap::Tilemap(float x, float y, std::vector<Tile> tiles) : tiles(tiles)
{
	this->position = { x, y };
}

void Tilemap::draw(RenderingAPI* API)
{
	for (int y = 0; y < TILEMAP_DIM_Y; y++)
	{
		for (int x = 0; x < TILEMAP_DIM_X; x++)
		{
			auto& tile = tiles[x + y * TILEMAP_DIM_X];
			API->addSpriteInstance
			(
				{ position.x + x * 2 * TILE_SIZE, position.y + y * 2 * TILE_SIZE },
				{ TILE_SIZE, TILE_SIZE },
				0,
				tile.textureRes->getContents(),
				tile.division
			);
		}
	}
	API->drawSpriteInstances();
}

void Tilemap::setTile(int x, int y, Tile tile)
{
	tiles[x + y * TILEMAP_DIM_X] = tile;
}

Tile::Tile(TileArchetype& archetype)
{
	this->division = archetype.division;
	this->passable = archetype.passable;
	this->textureRes = ResourceManager::getInstance().getResource<Texture>(archetype.texturePath);
}
