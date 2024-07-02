#include "Tilemap.hpp"

#include <nlohmann/json.hpp>
#include <fstream>

std::unordered_map<std::string, TileArchetype> TileArchetype::map = std::unordered_map<std::string, TileArchetype>();

void TileArchetype::initializeTileArchetypes()
{
	
}

Tilemap::Tilemap(float x, float y, int dimX, int dimY, float tileSize, std::vector<Tile> tiles)
	: tiles(tiles), dimensions({ dimX, dimY }), position({x, y}), tileSize(tileSize)
{
}

void Tilemap::draw(RenderingAPI* API)
{
	for (int y = 0; y < dimensions.y; y++)
	{
		for (int x = 0; x < dimensions.x; x++)
		{
			auto& tile = tiles[x + y * dimensions.x];
			API->addSpriteInstance
			(
				{ position.x + x * 2 * tileSize, position.y + y * 2 * tileSize },
				{ tileSize, tileSize },
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

nlohmann::json Tilemap::serialize()
{
	nlohmann::json j;

	for (auto& tile : tiles)
	{
		nlohmann::json jTile;
		jTile["division"] = { tile.division.x, tile.division.y, tile.division.z, tile.division.w };
		jTile["passable"] = tile.passable;
		jTile["texture"] = tile.textureRes->getContents()->getFilepath();

		j["tiles"].push_back(jTile);
	}

	j["position"] = { this->position.x, this->position.y };
	
	return j;
}

void Tilemap::deserialize(nlohmann::json j)
{
	this->position = { j["position"][0], j["position"][1] };

	tiles.clear();
	for (int i = 0; i < j["tiles"].size(); i++)
	{
		Tile tile(j["tiles"][i]["texture"], { j["tiles"][i]["division"][0], j["tiles"][i]["division"][1], j["tiles"][i]["division"][2], j["tiles"][i]["division"][3] }, j["tiles"][i]["passable"]);
		tiles.push_back(tile);
	}
}

Tile::Tile(TileArchetype& archetype)
{
	this->division = archetype.division;
	this->passable = archetype.passable;
	this->textureRes = ResourceManager::getInstance().getResource<Texture>(archetype.texturePath);
}

Tile::Tile(std::string textureFilepath, glm::vec4 division, bool passable) : division(division), passable(passable)	
{
	this->textureRes = ResourceManager::getInstance().getResource<Texture>(textureFilepath);
}
