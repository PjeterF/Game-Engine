#pragma once

#include "SysBase.hpp"

#include "../../Graphics/GeneralRenderer.hpp"

#include "../Components/Emitter.hpp"
#include "../Components/Transform.hpp"

class ParticleS : public SysBase
{
public:
	ParticleS();
	virtual void update(float dt) override;
	virtual void draw(RenderingAPI* rAPI);
};