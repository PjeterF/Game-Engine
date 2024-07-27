#include "Velocity.hpp"

nlohmann::json Velocity::serialize()
{
    nlohmann::json j;
    j["type"] = CBase::Velocity;

    j["velocity"] = { x, y };
    j["acceleration"] = { ax, ay };
    j["decay"] = decay;

    return j;
}

void Velocity::deSerialize(nlohmann::json j)
{
    x = j["velocity"][0];
    y = j["velocity"][1];
    ax = j["size"][0];
    ay = j["size"][1];
    decay = j["decay"];
}
