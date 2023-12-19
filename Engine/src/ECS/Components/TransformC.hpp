#pragma once

#include "ComponentBase.hpp"

class TransformC : public ComponentBase
{
public:
	TransformC(glm::vec2 position = { 0, 0 }, glm::vec2 size = {0, 0}, float rotation = 0);

	virtual void to_json(nlohmann::json& j) const;
	virtual void from_json(nlohmann::json& j);

	glm::vec2 position;
	glm::vec2 size;
	float rotation;
};