#include "ParticleEmitter.hpp"

ParticeEmitter::Particle::Particle(float x, float y, float xV, float yV, float particleSize, float initialRot, float r, float g, float b, float w)
{
	pos = glm::vec2(x, y);
	vel = glm::vec2(xV, yV);
	size = particleSize;
	rot = initialRot;
	col = glm::vec4(r, g, b, w);
}

ParticeEmitter::Particle::Particle()
{
}

ParticeEmitter::ParticeEmitter(float x, float y, int maxParticles)
{
	pos = glm::vec2(x, y);

	this->maxParticles = maxParticles;
	particlePool.resize(maxParticles);
}

ParticeEmitter::~ParticeEmitter()
{

}

void ParticeEmitter::update()
{
	for (int i = 0; i < particlePool.size(); i++)
	{
		if (!particlePool[i].emitted)
			continue;

		if (particlePool[i].lifetime <= 0)
		{
			particlePool[i].emitted = false;
			continue;
		}

		particlePool[i].pos = particlePool[i].pos + particlePool[i].vel;
		particlePool[i].col = (1-(float)particlePool[i].lifetime / (float)particlePool[i].maxLifetime) * (particlePool[i].endColour - particlePool[i].startColour)+ particlePool[i].startColour;
		particlePool[i].size = (1-(float)particlePool[i].lifetime / (float)particlePool[i].maxLifetime) * (particlePool[i].endSize - particlePool[i].startSize)+ particlePool[i].startSize;
		particlePool[i].vel = particlePool[i].velocityDecay * particlePool[i].vel;
		particlePool[i].lifetime--;
	}
}

void ParticeEmitter::draw(RenderingAPI* renderer)
{
	for (const auto& particle : particlePool)
	{
		if (particle.emitted)
			//renderer->quadRenderer->draw(particle.pos, glm::vec2(particle.size, particle.size), particle.rot, particle.col);
			renderer->addQuadInstance(particle.pos, { particle.size, particle.size }, particle.rot, particle.col);
	}
	renderer->drawQuadInstances();
}

void ParticeEmitter::emit()
{
	Particle& particle = particlePool[poolIndex];
	particle.emitted = true;
	particle.lifetime = randomInt(defaultProperties.particleLifetime.x, defaultProperties.particleLifetime.y);
	particle.maxLifetime = particle.lifetime;

	particle.pos = glm::vec2(pos.x + randomFloat(defaultProperties.xPosVar.x, defaultProperties.xPosVar.y), pos.y + randomFloat(defaultProperties.yPosVar.x, defaultProperties.yPosVar.y));
	particle.vel = glm::vec2(randomFloat(defaultProperties.xVelVar.x, defaultProperties.xVelVar.y), randomFloat(defaultProperties.yVelVar.x, defaultProperties.yVelVar.y));
	particle.size = defaultProperties.startSize;
	particle.startSize = defaultProperties.startSize;
	particle.endSize = defaultProperties.endSize;
	particle.rot = randomFloat(defaultProperties.rotVar.x, defaultProperties.rotVar.y);
	particle.col = defaultProperties.startColour;
	particle.startColour = defaultProperties.startColour;
	particle.endColour = defaultProperties.endColour;
	particle.velocityDecay = defaultProperties.velocityDecay;

	poolIndex = ++poolIndex % particlePool.size();
}

void ParticeEmitter::emit(ParticleProperties& particleProperties)
{
	Particle& particle = particlePool[poolIndex];
	particle.emitted = true;
	particle.lifetime = randomInt(particleProperties.particleLifetime.x, particleProperties.particleLifetime.y);
	particle.maxLifetime = particle.lifetime;

	particle.pos = glm::vec2(pos.x + randomFloat(particleProperties.xPosVar.x, particleProperties.xPosVar.y), pos.y + randomFloat(particleProperties.yPosVar.x, particleProperties.yPosVar.y));
	particle.vel = glm::vec2(randomFloat(particleProperties.xVelVar.x, particleProperties.xVelVar.y), randomFloat(particleProperties.yVelVar.x, particleProperties.yVelVar.y));
	particle.size = particleProperties.startSize;
	particle.startSize = particleProperties.startSize;
	particle.endSize = particleProperties.endSize;
	particle.rot = randomFloat(particleProperties.rotVar.x, particleProperties.rotVar.y);
	particle.col = particleProperties.startColour;
	particle.startColour = particleProperties.startColour;
	particle.endColour = particleProperties.endColour;
	particle.velocityDecay = particleProperties.velocityDecay;

	poolIndex = ++poolIndex % particlePool.size();
}

int ParticeEmitter::getParticleLimit()
{
	return maxParticles;
}

int ParticeEmitter::getParticleCount()
{
	return poolIndex;
}

void ParticeEmitter::applyForceInverseToSize(float xF, float yF)
{
	for (auto& particle : particlePool)
	{
		particle.vel = particle.vel + (1/particle.size) * glm::vec2(xF, yF);
	}
}

float ParticeEmitter::randomFloat(float rangeMin, float rangeMax)
{
	return rangeMin + (float)rand() / (float)RAND_MAX * (rangeMax - rangeMin);
}

int ParticeEmitter::randomInt(int rangeMin, int rangeMax)
{
	return rangeMin + rand() % (rangeMax - rangeMin + 1);
}
