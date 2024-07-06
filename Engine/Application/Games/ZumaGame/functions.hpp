#pragma once

#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "../../src/ECS2/Components/AABB.hpp"
#include "../../src/ECS2/Components/Animation.hpp"
#include "../../src/ECS2/Components/RenderingLayer.hpp"
#include "../../src/ECS2/Components/Sprite.hpp"
#include "../../src/ECS2/Components/Transform.hpp"
#include "../../src/ECS2/Components/Velocity.hpp"
#include "ECS/Components/MarbleComponent.hpp"
#include "../../src/ECS2/ComponentPoolManager.hpp"
#include "../../src/ECS2/EntityManager.hpp"

namespace ZumaFn
{
	int spawnMarble(std::string marbleArchetypeFilepath, glm::vec2 pos, std::string routeID);
}
