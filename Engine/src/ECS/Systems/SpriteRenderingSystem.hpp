#pragma once

#include "SystemBase.hpp"
#include "../../Graphics/GeneralRenderer.hpp"
#include "../Components/TransformC.hpp"
#include "../Components/SpriteC.hpp"

class SpriteRenderingSystem : public SystemBase
{
public:
	SpriteRenderingSystem(RenderingAPI* API);
	virtual void update(float dt) override;
private:
	RenderingAPI* API;
};