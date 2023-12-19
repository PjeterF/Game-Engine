#pragma once

#include "ComponentBase.hpp"
#include "../../Graphics/ParticleEmitter.hpp"

class ParticleC : public ComponentBase
{
public:
	ParticleC(float x = 0, float y = 0, int maxParticles = 100, int emitPeriod = 10, int emitCOunt = 1);

	virtual void to_json(nlohmann::json& j) const;
	virtual void from_json(nlohmann::json& j);

	ParticeEmitter emitter;
	int emitPeriod;
	int emitCount;
	int counter = 0;
};