#include "Animation.hpp"

nlohmann::json Animation::serialize()
{
    nlohmann::json j;
    j["type"] = CBase::Animation;

    j["frameDuration"] = frameDuration;
    j["state"] = state;
    j["currentFrame"] = currentFrame;

    j["frames"];
    for (auto& item : framesMap)
    {
        j["frames"][std::to_string(item.first)] = nlohmann::json::array();
        for (auto& division : item.second)
        {
            j["frames"][std::to_string(item.first)].push_back({ division.x, division.y, division.z, division.w });
        }
    }

    return j;
}

void Animation::deSerialize(nlohmann::json j)
{
    counter = 0;

    frameDuration = j["frameDuration"];
    state = j["state"];
    currentFrame = j["currentFrame"];

    //no finished
}
