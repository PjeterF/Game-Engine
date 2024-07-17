#include "ParticleS.hpp"

ParticleS::ParticleS()
{
	requiredComponents = {
		std::type_index(typeid(Emitter)),
		std::type_index(typeid(Transform))
	};
}

void ParticleS::update(float dt)
{
	auto emitterPool = ComponentPoolManager::getInstance().getPool<Emitter>();
	auto transformPool = ComponentPoolManager::getInstance().getPool<Transform>();

	for (auto ID : entities)
	{
		auto& emitter = emitterPool->get(ID);
		auto& transform = transformPool->get(ID);

		emitter.emitter.pos = { transform.x, transform.y };

		if (emitter.counter++ == emitter.emitPeriod)
		{
			emitter.counter = 0;
			
			for (int i = 0; i < emitter.emitCount; i++)
				emitter.emitter.emit();
		}

		emitter.emitter.update();
	}
}

void ParticleS::draw(RenderingAPI* rAPI)
{
	auto emitterPool = ComponentPoolManager::getInstance().getPool<Emitter>();

	for (auto ID : entities)
	{
		auto& emitter = emitterPool->get(ID);

		emitter.emitter.draw(rAPI);
	}
}
