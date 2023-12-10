#include "ShortLivedParticleEmitter.hpp"

ParticleBurst::ParticleBurst(float x, float y, int maxParticles, int emitterLifespan) : ParticeEmitter(x, y, maxParticles)
{
	type = emitter;

	this->emitterLifespan = emitterLifespan;

	ParticleProperties props = ParticleProperties();
	float vel = 2.5;
	props.xVelVar =glm::vec2(-vel, vel);
	props.yVelVar =glm::vec2(-vel, vel);
	props.particleLifetime = glm::vec2(10, 30);

	this->defaultProperties = props;

	for (int i = 0 ; i < maxParticles; i++)
	{
		ParticeEmitter::emit();
	}
}

ParticleBurst::ParticleBurst(float x, float y, int maxParticles, int emitterLifespan, ParticeEmitter::ParticleProperties properties) : ParticeEmitter(x, y, maxParticles)
{
	type = emitter;
	this->emitterLifespan = emitterLifespan;
	this->defaultProperties = properties;

	for (int i = 0; i < maxParticles; i++)
	{
		ParticeEmitter::emit();
	}
}

void ParticleBurst::update()
{
	if (--emitterLifespan <= 0)
	{
		this->destroy();
		return;
	}
	ParticeEmitter::update();
}

void ParticleBurst::draw(RenderingAPI* renderer)
{
	ParticeEmitter::draw(renderer);
}
