#pragma once

#include "../Graphics/ParticleEmitter.hpp"

class ParticleBurst : public Entity, public ParticeEmitter
{
public:
	ParticleBurst(float x, float y, int maxParticles, int emitterLifespan);
	ParticleBurst(float x, float y, int maxParticles, int emitterLifespan, ParticeEmitter::ParticleProperties properties);
	virtual void update();
	virtual void draw(RenderingAPI* renderer) override;
private:
	int emitterLifespan;
};