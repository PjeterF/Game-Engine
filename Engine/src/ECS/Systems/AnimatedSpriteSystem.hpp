#pragma once

#include "SystemBase.hpp"
#include "../../Graphics/GeneralRenderer.hpp"

class AnimatedSpriteSystem : public SystemBase
{
public:
	AnimatedSpriteSystem(RenderingAPI* API);
	virtual void update(float dt) override;
private:
	RenderingAPI* API;
};