#include "Transform.hpp"

nlohmann::json Transform::serialize()
{
    nlohmann::json j;

    j["position"] = { x, y };
    j["size"] = { width, height };
    j["rot"] = rot;
    j["rotateToDir"] = rotateToDir;

    return j;
}

void Transform::deSerialize(nlohmann::json j)
{
    x = j["position"][0];
    y = j["position"][1];
    width = j["size"][0];
    height = j["size"][1];
    rot = j["rot"];
    rotateToDir = j["rotateToDir"];
}
