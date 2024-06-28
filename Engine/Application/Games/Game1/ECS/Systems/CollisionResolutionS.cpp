#include "CollisionResolutionS.hpp"

CollisionResolutionS::CollisionResolutionS()
{
	requiredComponents = {
		std::type_index(typeid(Transform)),
		std::type_index(typeid(AABB)),
		std::type_index(typeid(CharacterStats))
	};
}

void CollisionResolutionS::update(float dt)
{
	auto colPool = ComponentPoolManager::getInstance().getPool<AABB>();
	auto statsPool = ComponentPoolManager::getInstance().getPool<CharacterStats>();
	auto& entManager = EntityManager::getInstance();

	for (auto ID : entities)
	{
		auto& col = colPool->get(ID);
		auto& stats = statsPool->get(ID);
		auto tag = entManager.getTag(ID);

		for (auto otherID : col.collidingEntIDs)
		{
			auto& otherStats = statsPool->get(otherID);
			auto otherTag = entManager.getTag(otherID);

			switch (tag)
			{
			case DefaultTag:
				break;
			case Barrier:
				break;
			case PlayerCharacter:
				if (otherTag == Enemy || otherTag == Projectile)
				{
					stats.currenthealth = stats.currenthealth - otherStats.attackDamage;
				}

				break;
			case Enemy:
				if(otherTag==Projectile)
					stats.currenthealth = stats.currenthealth - otherStats.attackDamage;
				break;
			case Projectile:
				if (otherTag != Projectile)
					entManager.deleteEntity(ID);
				break;
			default:
				break;
			}

			if (stats.currenthealth < 0)
				entManager.deleteEntity(ID);
		}
	}
}

void CollisionResolutionS::drawHealthBars(RenderingAPI* API)
{
	glm::vec2 barDim = { 200, 5 };

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

		API->addQuadInstance({ trans.x, trans.y + col.height }, { 200, 10 }, 0, { 0, 1, 0, 1 });
	}
	API->drawQuadInstances();
}
