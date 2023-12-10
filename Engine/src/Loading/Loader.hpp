#pragma once

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "../Managers/TexturesManager.hpp"
#include "../Managers/EntityManager.hpp"

class Loader
{
public:
	Loader() {}

	static void serializeTextures(TexturesManager* manager, std::string filename);
	static void serializeScene(EntityManager* manager, std::string filename);

	static void deserializeScene(EntityManager* manager, std::string filepath);
};