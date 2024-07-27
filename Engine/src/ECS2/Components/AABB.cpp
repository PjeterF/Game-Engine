#include "AABB.hpp"

nlohmann::json AABB::serialize()
{
    nlohmann::json j;
    j["type"] = CBase::AABB;

    j["size"] = { width, height };
    j["mass"] = mass;
    j["enabled"] = enabled;

    return j;
}

void AABB::deSerialize(nlohmann::json j)
{
    width = j["size"][0];
    height = j["size"][1];
    mass = j["mass"];
    enabled = j["enabled"];

    collidingEntIDs.clear();
}
