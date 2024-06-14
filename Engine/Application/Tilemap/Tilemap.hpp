#pragma once

#include "../../src/Graphics/GeneralRenderer.hpp"

#include <vector>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>

#include <nlohmann/json.hpp>

#define TILEMAP_DIM_X 5
#define TILEMAP_DIM_Y 5
#define TILE_ARRAY_SIZE TILEMAP_DIM_X*TILEMAP_DIM_Y
#define TILE_SIZE 50

enum TileType
{
	Path1='P1', Grass = 'G', Water = 'W'
};

struct TileArchetype
{
	TileArchetype() {}
	TileArchetype(std::string texturePath, glm::vec4 division, bool passable) : texturePath(texturePath), division(division), passable(passable) {}
	std::string texturePath;
	glm::vec4 division;
	bool passable;
	static std::unordered_map<TileType, TileArchetype> map;
	static void initializeTileArchetypes();
};

struct Tile
{
	Tile(TileArchetype& archetype);
	Tile(std::string textureFilepath, glm::vec4 division, bool passable);
	Resource<Texture>* textureRes;
	glm::vec4 division;
	bool passable;
};

class Tilemap
{
public:
	Tilemap(float x, float y, std::vector<Tile> tiles);
	void draw(RenderingAPI* API);
	void setTile(int x, int y, Tile tile);
	nlohmann::json serialize();
	void deserialize(nlohmann::json j);
private:
	glm::vec2 position;
	std::vector<Tile> tiles;
	std::vector<glm::vec4> staticColliders;
};