#include "MarbleShooter.hpp"

nlohmann::json MarbleShooter::serialize()
{
    nlohmann::json j;
    j["type"] = CBase::MarbleShooter;

    j["cooldown"] = cooldown;
    j["counter"] = counter;
    j["shotSpeed"] = shotSpeed;

    return j;
}

void MarbleShooter::deSerialize(nlohmann::json j)
{
    cooldown = j["cooldown"];
    counter = j["counter"];
    shotSpeed = j["shotSpeed"];
}
