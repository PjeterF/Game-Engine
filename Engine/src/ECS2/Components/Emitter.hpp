#pragma once

#include "../../Graphics/ParticleEmitter.hpp"

struct Emitter
{
	Emitter(int emitPeriod = 1, int startingCounterValue = 1, int emitCount = 0, int maxParticles=0) : emitPeriod(emitPeriod), emitCount(emitCount), counter(startingCounterValue)
	{
		emitter = ParticeEmitter(0, 0, maxParticles);
	}
	ParticeEmitter emitter;
	int emitPeriod;
	int emitCount;
	int counter;
};