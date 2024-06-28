#pragma once

#include "../../Managers/Resource/ResourceManager.hpp"
#include <glm/glm.hpp>

struct RenderingLayer
{
	RenderingLayer(int layer = 0) : layer(layer) {}
	int layer;
};