#include "VelocityC.hpp"

VelocityC::VelocityC(glm::vec2 velocity) : ComponentBase(CType::Velocity)
{
	this->velocity = velocity;
}

nlohmann::json VelocityC::serialize()
{
	nlohmann::json j = ComponentBase::serialize();

	j["velocity"] = { velocity.x, velocity.y };

	return j;
}

void VelocityC::to_json(nlohmann::json& j) const
{
	ComponentBase::to_json(j);
	j["velocity"] = { velocity.x, velocity.y };
}

void VelocityC::from_json(nlohmann::json& j)
{
	this->velocity = { j["velocity"][0], j["velocity"][1] };
}
