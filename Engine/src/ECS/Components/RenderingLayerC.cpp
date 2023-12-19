#include "RenderingLayerC.hpp"

RenderingLayerC::RenderingLayerC(int layer) : ComponentBase(RenderingLayer)
{
	this->layer = layer;
}

void RenderingLayerC::to_json(nlohmann::json& j) const
{
	ComponentBase::to_json(j);

	j["layer"] = this->layer;
}

void RenderingLayerC::from_json(nlohmann::json& j)
{
	this->layer = j["layer"];
}
