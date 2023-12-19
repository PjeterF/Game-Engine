#include "TransformC.hpp"

TransformC::TransformC(glm::vec2 position, glm::vec2 size, float rotation) : ComponentBase(CType::Transform)
{
	this->position = position;
	this->size = size;
	this->rotation = rotation;
}

void TransformC::to_json(nlohmann::json& j) const
{
	ComponentBase::to_json(j);
	j["position"] = { position.x, position.y };
	j["size"] = { size.x, size.y };
	j["rotation"] = rotation;
}

void TransformC::from_json(nlohmann::json& j)
{
	this->position = { j["position"][0], j["position"][1] };
	this->size = { j["size"][0], j["size"][1] };
	this->rotation = j["rotation"];
}
