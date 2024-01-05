#include "serialization.hpp"

#include "../Logic/RouteManagementSystem.hpp"
#include "../Logic/ShooterManagementSystem.hpp"
#include "../../src/ECS/Systems/CollisionSystem.hpp"
#include "../../src/ECS/Systems/LayeredRenderingSystem.hpp"
#include "../../src/ECS/Systems/MovementSystem.hpp"
#include "../../src/ECS/Systems/ParticleSystem.hpp"
#include "../../src/ECS/Systems/CounterKillerSystem.hpp"

#include <iostream>
#include <fstream>

bool serializeScene(std::string filepath, EntManager& entityManager, SystemsManager& systemsManager)
{
    nlohmann::json j_ent;
    nlohmann::json j_sys;
    nlohmann::json j_temp;

    j_ent["entities"] = nlohmann::json::array();
    j_sys["systems"] = nlohmann::json::array();
    j_temp["marbleTemplates"] = nlohmann::json::array();
    j_ent["entManager_nextID"] = entityManager.nextID;

    auto it = entityManager.begin();
    while (it != entityManager.end())
    {
        nlohmann::json j;

        if (!(*it).second->hasComponent(RouteInfo))
        {
            (*it).second->to_json(j);
            j_ent["entities"].push_back(j);
        }

        it++;
    }

    for (auto& set : systemsManager.systemSets)
    {
        for (auto& system : set.second)
        {
            nlohmann::json j;

            system.second->to_json(j);
            if(!j.is_null())
                j_sys["systems"].push_back(j);
        }
    }

    for (auto& mTemplate : RouteManagementSystem::marbleTemplates)
    {
        nlohmann::json j;

        j["size"] = mTemplate.size;
        j["tag"] = mTemplate.tag;
        j["textureFilepath"] = mTemplate.textureFilepath;
        j["frameDuration"] = mTemplate.frameDuration;
        j["explosionColor"] = { mTemplate.explosionColor.x, mTemplate.explosionColor.y, mTemplate.explosionColor.z };
        
        j["divisions"] = nlohmann::json::array();
        for (auto& division : mTemplate.divisions)
            j["divisions"].push_back({ division.x, division.y, division.width, division.height });

        j_temp["marbleTemplates"].push_back(j);
    }

    j_ent.merge_patch(j_sys);
    j_ent.merge_patch(j_temp);

    std::ofstream file(filepath);
    file << j_ent.dump(4);

    return false;
}

bool deSerializeScene(std::string filepath, EntManager& entityManager, SystemsManager& systemsManager)
{
    entityManager.deleteAllEntities();
    systemsManager.deleteAllNonPermSystems();
    RouteManagementSystem::marbleTemplates.clear();

    nlohmann::json j;
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cout << "Error opening file!\n";
        return false;
    }

    file >> j;
    std::cout<< j.dump(4);

    for (auto j_ent : j["entities"])
    {
        Ent* newEntity = new Ent(-1);
        newEntity->from_json(j_ent);
        entityManager.addEntity(newEntity);
    }

    for (auto& j_temp : j["marbleTemplates"])
    {
        std::vector<TextureDivision> divisions;
        for (auto& division : j_temp["divisions"])
            divisions.push_back(TextureDivision(division[0], division[1], division[2], division[3]));

        RouteManagementSystem::marbleTemplates.push_back
        (
            MarbleTemplate
            (
                j_temp["size"],
                j_temp["tag"],
                j_temp["textureFilepath"],
                divisions,
                j_temp["frameDuration"],
                { j_temp["explosionColor"][0], j_temp["explosionColor"][1], j_temp["explosionColor"][2] }
            )
        );
    }

    for (auto j_sys : j["systems"])
    {
        int type = j_sys["type"];
        switch (type)
        {
        case RouteManagementSys:
        {
            RouteManagementSystem* newSys = new RouteManagementSystem(nullptr);
            newSys->from_json(j_sys);
        }
        break;
        case ShooterManagementSys:
        {
            ShooterManagementSystem* newSys = new ShooterManagementSystem();
            newSys->from_json(j_sys);
        }
        break;
        case CollisionSys:
        {
            CollisionSystem::getInstance().from_json(j_sys);
        }
        break;
        case LayeredRenderingSys:
        {
            LayeredRenderingSystem::getInstance().from_json(j_sys);
        }
        break;
        case MovementSys:
        {
            MovementSystem::getInstance().from_json(j_sys);
        }
        break;
        case ParticleSys:
        {
            ParticleSystem::getInstance().from_json(j_sys);
        }
        break;
        case CounterKillerSys:
        {
            CounterKillerSystem::getInstance().from_json(j_sys);
        }
        break;
        }
    }

    return false;
}
