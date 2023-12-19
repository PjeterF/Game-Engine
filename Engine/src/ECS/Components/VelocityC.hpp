#pragma once

#include "ComponentBase.hpp"

class VelocityC : public ComponentBase
{
public:
	VelocityC(glm::vec2 velocity = { 0, 0 });

	virtual void to_json(nlohmann::json& j) const;
	virtual void from_json(nlohmann::json& j);

	glm::vec2 velocity;
};