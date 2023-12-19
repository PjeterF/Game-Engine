#include "ShooterInfoC.hpp"

ShooterC::ShooterC(float shotSpeed, int cooldown) : ComponentBase(ShooterInfo)
{
	this->shotSpeed = shotSpeed;
	this->cooldownTime = cooldown;
}

void ShooterC::to_json(nlohmann::json& j) const
{
	ComponentBase::to_json(j);
	j["shotSpeed"] = shotSpeed;
	j["cooldownTime"] = cooldownTime;
}

void ShooterC::from_json(nlohmann::json& j)
{
	shotSpeed = j["shotSpeed"];
	cooldownTime = j["cooldownTime"];
}
