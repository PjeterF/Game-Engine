#pragma once

#include "../Managers/Entity.hpp"
#include <random>

class ParticeEmitter
{
public:

	struct Particle
	{
		Particle(float x, float y, float xV, float yV, float size, float rot, float r, float g, float b, float w);
		Particle();

		glm::vec2 pos;
		glm::vec2 vel;
		float size;
		float startSize;
		float endSize;
		float rot;
		glm::vec4 col;
		glm::vec4 startColour;
		glm::vec4 endColour;
		int lifetime=0;
		int maxLifetime = 0;
		bool emitted = false;
		float velocityDecay;
	};
	struct ParticleProperties
	{
		glm::vec2 xVelVar = glm::vec2(-0.5, 0.5), yVelVar = glm::vec2(-0.5, 0.5);
		glm::vec2 xPosVar = glm::vec2(0, 0), yPosVar = glm::vec2(0, 0);
		glm::vec4 startColour = glm::vec4(0.8, 0.8, 0.8, 0.7), endColour = glm::vec4(0.6, 0.2, 0.1, 0.7);
		glm::vec2 rotVar = glm::vec2(0, 360);
		float startSize = 3;
		float endSize = 0;
		glm::vec2 particleLifetime = glm::vec2(50, 100);
		float velocityDecay = 1;
	};
	ParticeEmitter(float x, float y, int maxParticles);
	virtual ~ParticeEmitter();
	virtual void update();
	virtual void draw(RenderingAPI* renderer);
	void emit();
	void emit(ParticleProperties& particleProperties);
	int getParticleLimit();
	int getParticleCount();

	void applyForceInverseToSize(float xF, float yF);

	ParticleProperties defaultProperties = ParticleProperties();

	glm::vec2 pos;
private:
	float randomFloat(float rangeMin, float rangeMax);
	int randomInt(int rangeMin, int rangeMax);
	int maxParticles;
	std::vector<Particle> particlePool;
	int poolIndex = 0;
};