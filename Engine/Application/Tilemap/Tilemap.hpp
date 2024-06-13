#pragma once

#include "../../src/Graphics/GeneralRenderer.hpp"

#include <vector>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>

#define TILEMAP_DIMENSIONS 16
#define TILE_SIZE 10

enum TileType
{
	Dirt='D', Grass = 'G', Water = 'W'
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
	TileType type;
	Resource<Texture>* textureRes;
	glm::vec4 division;
	bool passable;
};

class Tilemap
{
public:
	Tilemap(float x, float y, std::vector<TileType> tiles);
	void draw(RenderingAPI* API);
private:
	glm::vec2 position;
	std::vector<TileType> tiles;
	std::vector<glm::vec4> staticColliders;
};