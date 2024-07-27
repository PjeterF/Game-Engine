#pragma once

#include "../../Managers/Resource/ResourceManager.hpp"
#include <glm/glm.hpp>

#include "CBase.hpp"

struct RenderingLayer :public CBase
{
	RenderingLayer(int layer = 0) : layer(layer) {}
	int layer;

	virtual nlohmann::json serialize() override;
	virtual void deSerialize(nlohmann::json j) override;
};