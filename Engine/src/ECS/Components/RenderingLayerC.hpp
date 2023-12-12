#pragma once

#include "ComponentBase.hpp"

class RenderingLayerC : public ComponentBase
{
public:
	RenderingLayerC(int layer = 0);
	int layer = 0;
};