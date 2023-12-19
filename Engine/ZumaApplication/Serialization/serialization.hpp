#pragma once

#include "../../src/ECS/Systems/SystemsManager.hpp"
#include "../../src/ECS/Entity/EntManager.hpp"

bool serializeScene(std::string filepath, EntManager& entityManager, SystemsManager& systemsManager);
bool deSerializeScene(std::string filepath, EntManager& entityManager, SystemsManager& systemsManager);