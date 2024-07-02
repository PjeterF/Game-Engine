#include "CollisionResolutionS.hpp"

CollisionResolutionS::CollisionResolutionS()
{
	requiredComponents = {
		std::type_index(typeid(Transform)),
		std::type_index(typeid(AABB)),
		std::type_index(typeid(Velocity)),
		std::type_index(typeid(CharacterStats))
	};
}

void CollisionResolutionS::update(float dt)
{
	auto colPool = ComponentPoolManager::getInstance().getPool<AABB>();
	auto statsPool = ComponentPoolManager::getInstance().getPool<CharacterStats>();
	auto velPool = ComponentPoolManager::getInstance().getPool<Velocity>();
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
			case PlayerCharacter:
				if (otherTag == Enemy || otherTag == EnemyProjectile)
					stats.currenthealth = stats.currenthealth - otherStats.collisionDamage;
				break;
			case Enemy:
				if (otherTag == FriendlyProjectile)
				{
					stats.currenthealth = stats.currenthealth - otherStats.collisionDamage;
					auto& vel = velPool->get(ID);
					vel.x = -3*vel.x;
					vel.y = -3 * vel.y;
				}
				break;
			case FriendlyProjectile:
				if (otherTag != FriendlyProjectile && otherTag != EnemyProjectile && otherTag != PlayerCharacter)
					entManager.deleteEntity(ID);
				break;
			case EnemyProjectile:
				if (otherTag != FriendlyProjectile && otherTag != EnemyProjectile && otherTag != Enemy)
					entManager.deleteEntity(ID);
				break;
			}

			if (stats.currenthealth < 0)
				entManager.deleteEntity(ID);
		}
	}
}
