#pragma once

#include "SysBase.hpp"

#include "../../Graphics/GeneralRenderer.hpp"

#include "../Components/Transform.hpp"
#include "../Components/Sprite.hpp"

class RenderingS : public SysBase
{
public:
	RenderingS(RenderingAPI* rAPI);
	void update(float dt) override;
private:
	RenderingAPI* rAPI;
};