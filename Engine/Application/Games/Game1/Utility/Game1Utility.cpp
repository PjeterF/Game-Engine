#include "Game1Utility.hpp"

int spawnCharacter(std::string filepath, glm::vec2 pos)
{
	std::ifstream file(filepath);
	if (!file.is_open())
	{
		file.close();
		return -1;
	}

	nlohmann::json j = nlohmann::json::parse(file);

	Entity newEnt = EntityManager::getInstance().createEntity();
	newEnt.addComponent<Transform>(Transform(pos.x, pos.y, j["size"], j["size"]));
	newEnt.addComponent<Velocity>(Velocity());
	newEnt.addComponent<AABB>(AABB(j["colliderSize"], j["colliderSize"], j["mass"]));
	newEnt.addComponent<CharacterStats>(CharacterStats(j["maxHealth"], j["maxHealth"], j["collisionDamage"], j["attackDamage"], j["speed"]));
	EntityManager::getInstance().setTag(newEnt.getID(), j["tag"]);

	std::map<Animation::State, std::vector<glm::vec4>> framesMap;
	for (auto& item : j["animationFrames"].items())
	{
		Animation::State state = (Animation::State)std::stoi(item.key());
		std::vector<glm::vec4> divisions;
		auto& array = item.value();
		for (int i = 0; i < array.size(); i++)
		{
			divisions.push_back({ array.at(i).at(0), array.at(i).at(1), array.at(i).at(2), array.at(i).at(3) });
		}
		framesMap[state] = divisions;
	}

	newEnt.addComponent<Animation>(Animation(
		framesMap,
		Animation::run,
		j["frameDuration"]
	));
	auto it = framesMap.begin();
	auto div = (*it).second[0];
	newEnt.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>(j["texture"]), div));

	return newEnt.getID();
}
