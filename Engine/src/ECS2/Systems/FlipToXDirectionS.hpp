#pragma once

#include "SysBase.hpp"

#include "../../Graphics/GeneralRenderer.hpp"

#include "../Components/Sprite.hpp"
#include "../Components/Velocity.hpp"

class FlipSpriteToXDirectionS : public SysBase
{
public:
	FlipSpriteToXDirectionS();
	virtual void update(float dt) override;
};