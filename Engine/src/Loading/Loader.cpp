#include "Loader.hpp"
#include "../ZumaLogic/Sprite.hpp"
#include "../ZumaLogic/Shooter.hpp"
#include "../ZumaLogic/Snake.hpp"
#include "../ZumaLogic/Route.hpp"

void Loader::serializeTextures(TexturesManager* manager, std::string filename)
{
	using json = nlohmann::json;

	auto it = manager->begin();

	json j;

	while (it != manager->end())
	{
		j["Textures"].push_back({ { "name", (*it).first }, { "filepath", (*it).second.path } });
		it++;
	}

	std::ofstream file(filename);
	file << std::setw(4) << j;
}

void Loader::serializeScene(EntityManager* manager, std::string filename)
{
	nlohmann::json j;

	int i = 0;
	for (auto layer : manager->layers)
	{
		nlohmann::json jsonLayer;

		for (auto entity : layer->entities)
		{
			switch (entity.second->type)
			{
			case Entity::EntityType::route:
			{
				Route* ent = (Route*)entity.second;
				jsonLayer["Routes"].push_back(ent->serialize());
			}
			break;
			case Entity::EntityType::shooter:
			{
				Shooter* ent = (Shooter*)entity.second;
				jsonLayer["Shooters"].push_back(ent->serialize());
			}
			break;
			case Entity::EntityType::sprite:
			{
				Sprite* ent = (Sprite*)entity.second;
				jsonLayer["Sprites"].push_back(ent->serialize());
			}
			break;
			default:
				break;
			}
		}
		if (jsonLayer.is_null())
			j["Layers"].push_back({});
		else
			j["Layers"].push_back(jsonLayer);
		i++;
	}
	std::cout << j.dump(4);
	for (auto segTemplate : Segment::segmentTemplates)
	{
		j["SegmentTemplates"].push_back(segTemplate.serialize());
	}
	std::cout << j.dump(4);


	std::ofstream file(filename);
	file<< std::setw(4) << j;
}

void Loader::deserializeScene(EntityManager* manager, std::string filepath)
{
	std::ifstream file(filepath);
	nlohmann::json j;

	if (file.is_open())
	{
		try
		{
			j = nlohmann::json::parse(file);
		}
		catch (nlohmann::json::parse_error& ex)
		{
			std::cerr << "parse error at byte " << ex.byte << std::endl;
		}
	}
	else
	{
		return;
	}

	EntityManager::getInstance().deleteAll();

	if (!j["SegmentTemplates"].is_null())
	{
		Segment::segmentTemplates.clear();
		for (auto it = j["SegmentTemplates"].begin(); it != j["SegmentTemplates"].end(); it++)
		{
			SegmentTemplate* segTemplate = SegmentTemplate::deserialize(*it);
			Segment::segmentTemplates.push_back(*segTemplate);
		}
	}

	int i = 0;
	for (auto it_layer = j["Layers"].begin(); it_layer != j["Layers"].end(); it_layer++)
	{
		if (!(*it_layer)["Routes"].is_null())
		{
			for (auto it = (*it_layer)["Routes"].begin(); it != (*it_layer)["Routes"].end(); it++)
			{
				if (!(*it).is_null())
				{
					Route* route = Route::deSerialize(*it);
					manager->update();
					route->setLayer(i);
				}
			}
		}

		if (!(*it_layer)["Shooters"].is_null())
		{
			for (auto it = (*it_layer)["Shooters"].begin(); it != (*it_layer)["Shooters"].end(); it++)
			{
				if (!(*it).is_null())
				{
					Shooter* shooter = Shooter::deSerialize((*it));
					manager->update();
					shooter->setLayer(i);
				}
			}
		}
		
		if (!(*it_layer)["Sprites"].is_null())
		{
			for (auto it = (*it_layer)["Sprites"].begin(); it != (*it_layer)["Sprites"].end(); it++)
			{
				if (!(*it).is_null())
				{
					Sprite* sprite = Sprite::deSerialize((*it));
					manager->update();
					sprite->setLayer(i);
				}
			}
		}
		i++;
	}
	EntityManager::getInstance().update();
}
