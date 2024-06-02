#include "ParticleSystem.hpp"

#include "../Components/ParticleC.hpp"
#include "../Components/TransformC.hpp"

#include "../Entity/EntManager.hpp"

ParticleSystem::ParticleSystem() : SystemBase(UNPAUSED, true, ParticleSys)
{
	name = "ParticleSys(" + std::to_string(ID) + ")";

	requiredComponents = { Transform_, Particle };
}

ParticleSystem& ParticleSystem::getInstance()
{
	static ParticleSystem system;
	return system;
}

void ParticleSystem::to_json(nlohmann::json& j) const
{
	j["type"] = type;
	j["entIDs"] = nlohmann::json::array();
	for (auto& ent : entities)
		j["entIDs"].push_back(ent->getID());
}

void ParticleSystem::from_json(nlohmann::json& j)
{
	for (auto& entID : j["entIDs"])
	{
		Ent* ent = EntManager::getInstance().getEntity(entID);
		if (ent != nullptr)
			this->addEntity(ent);
	}
}

void ParticleSystem::update(float dt)
{
	for (auto& entity : entities)
	{
		auto transform = (TransformC*)entity->getComponent(Transform_);
		auto emitter = (ParticleC*)entity->getComponent(Particle);

		if (emitter->counter--==0)
		{
			for (int i = 0; i < emitter->emitCount; i++)
				emitter->emitter.emit();

			emitter->counter = emitter->emitPeriod;
		}

		emitter->emitter.pos = transform->position;
		emitter->emitter.update();
	}
}
