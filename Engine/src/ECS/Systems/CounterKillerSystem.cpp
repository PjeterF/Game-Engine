#include "CounterKillerSystem.hpp"

#include "../Entity/EntManager.hpp"

CounterKillerSystem& CounterKillerSystem::getInstance()
{
    static CounterKillerSystem system;
    return system;
}

void CounterKillerSystem::to_json(nlohmann::json& j) const
{
    j["type"] = type;
    j["entIDs"] = nlohmann::json::array();
    for (auto& ent : entities)
        j["entIDs"].push_back(ent.second->getID());
}

void CounterKillerSystem::from_json(nlohmann::json& j)
{
    for (auto& entID : j["entIDs"])
    {
        Ent* ent = EntManager::getInstance().getEntity(entID);
        if (ent != nullptr)
            this->getInstance().addEntity(ent);
    }
}

void CounterKillerSystem::update(float dt)
{
    for (auto& entity : entities)
    {
        auto counter = (CounterC*)entity.second->getComponent(Counter);

        if (counter->counter++ >= counter->originalValue)
            entity.second->destroy();
    }
}

CounterKillerSystem::CounterKillerSystem() : SystemBase(UNPAUSED, true, CounterKillerSys)
{
    name = "CounterKillerSys(" + std::to_string(ID) + ")";

    requiredComponents = { Counter };
}
