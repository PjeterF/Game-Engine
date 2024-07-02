#include "HealthBarDrawingS.hpp"

HealthBarDrawingS::HealthBarDrawingS(RenderingAPI* rAPI) : rAPI(rAPI)
{
}

void HealthBarDrawingS::update(float dt)
{
	auto transPool = ComponentPoolManager::getInstance().getPool<Transform>();
	auto colPool = ComponentPoolManager::getInstance().getPool<AABB>();
	auto statsPool = ComponentPoolManager::getInstance().getPool<CharacterStats>();

	for (auto ID : entities)
	{
		auto& trans = transPool->get(ID);
		auto& col = colPool->get(ID);
		auto& stats = statsPool->get(ID);

		float healthWidth = barDim.x * stats.currenthealth / stats.maxHealth;
		float depletedWidth = barDim.x - healthWidth;

		rAPI->addQuadInstance({ trans.x, trans.y + col.height }, { barDim.x, barDim.y }, 0, { 1, 0.3, 0.05, 1 });
		rAPI->addQuadInstance({ trans.x, trans.y + col.height }, { healthWidth, barDim.y }, 0, { 0.4, 0.75, 0.08, 1 });
	}
	rAPI->drawQuadInstances();
}
