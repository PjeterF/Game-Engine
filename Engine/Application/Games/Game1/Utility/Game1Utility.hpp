#pragma once

#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

#include "../../src/ECS2/EntityManager.hpp"
#include "../../src/ECS2/ComponentPoolManager.hpp"
#include "../../src/Managers/Resource/ResourceManager.hpp"

#include "../../src/ECS2/Components/AABB.hpp"
#include "../../src/ECS2/Components/Animation.hpp"
#include "../../src/ECS2/Components/Transform.hpp"
#include "../../src/ECS2/Components/Velocity.hpp"
#include "../../src/ECS2/Components/Sprite.hpp"

#include "../ECS/Components/CharacterStats.hpp"

int spawnCharacter(std::string filepath, glm::vec2 pos);

int spawnProjectile(std::string archetypeFilePath, glm::vec2 pos);