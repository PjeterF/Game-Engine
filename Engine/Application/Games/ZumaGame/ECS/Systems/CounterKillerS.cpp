#include "CounterKillerS.hpp"

CounterKillerS::CounterKillerS()
{
	requiredComponents = {
		std::type_index(typeid(Counter))
	};
}

void CounterKillerS::update(float dt)
{
	auto counterPool = ComponentPoolManager::getInstance().getPool<Counter>();

	for (auto ID : entities)
	{
		auto& counter = counterPool->get(ID);

		if (counter.val++ >= counter.maxValue)
		{
			counter.val = 0;
			EntityManager::getInstance().deleteEntity(ID);
		}
	}
}
