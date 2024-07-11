#include "functions.hpp"

namespace ZumaFn
{
	int spawnMarble(std::string marbleArchetypeFilepath, glm::vec2 pos, std::string routeID)
	{
		std::ifstream file(marbleArchetypeFilepath);
		if (!file.is_open())
		{
			file.close();
			return -1;
		}
		nlohmann::json j = nlohmann::json::parse(file);

		Entity ent = EntityManager::getInstance().createEntity();

		ent.addComponent<Transform>(Transform(pos.x, pos.y, j["size"], j["size"], 0, true));
		ent.addComponent<Velocity>(Velocity());
		ent.addComponent<AABB>(AABB(j["size"], j["size"]));

		std::vector<glm::vec4> frames;
		for (int i = 0; i < j["frames"].size(); i++)
			frames.push_back({ j["frames"][i][0], j["frames"][i][1], j["frames"][i][2], j["frames"][i][3] });

		std::map<Animation::State, std::vector<glm::vec4>> framesMap;
		if(frames.empty())
			framesMap[Animation::idle] = {};
		else
			framesMap[Animation::idle] = { frames[0] };
		framesMap[Animation::run] = frames;
		ent.addComponent<Animation>(Animation(
			framesMap,
			Animation::idle,
			j["frameDuration"]
		));

		if (frames.empty())
			ent.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>(j["texture"])));
		else
			ent.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>(j["texture"]), frames[0]));
		ent.addComponent<RenderingLayer>(RenderingLayer());
		ent.addComponent<MarbleComponent>(MarbleComponent(j["type"], routeID));

		return ent.getID();
	}

}

