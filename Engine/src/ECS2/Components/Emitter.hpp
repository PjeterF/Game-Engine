#pragma once

#include "../../Graphics/ParticleEmitter.hpp"
#include "CBase.hpp"

struct Emitter : public CBase
{
	Emitter(int emitPeriod = 1, int startingCounterValue = 1, int emitCount = 0, int maxParticles=0) : emitPeriod(emitPeriod), emitCount(emitCount), counter(startingCounterValue)
	{
		emitter = ParticeEmitter(0, 0, maxParticles);
	}
	ParticeEmitter emitter;
	int emitPeriod;
	int emitCount;
	int counter;

	virtual nlohmann::json serialize();
	virtual void deSerialize(nlohmann::json j);
};