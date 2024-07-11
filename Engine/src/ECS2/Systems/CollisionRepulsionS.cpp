#include "CollisionRepulsionS.hpp"
#include "../Components/Transform.hpp"
#include "../Components/AABB.hpp"
#include "../Components/Velocity.hpp"

#include <glm/glm.hpp>

CollisionRepulsionS::CollisionRepulsionS()
{
	requiredComponents = { std::type_index(typeid(Transform)), std::type_index(typeid(AABB)), std::type_index(typeid(Velocity)) };
}

void CollisionRepulsionS::update(float dt)
{
	for (auto ID : entities)
	{
		Entity ent1(ID);
		auto& col1 = ent1.getComponent<AABB>();
		for (auto ID2 : col1.collidingEntIDs)
		{
			auto tag2 = EntityManager::getInstance().getTag(ID2);

			Entity ent2(ID2);
			auto& trans1 = ent1.getComponent<Transform>();
			auto& trans2 = ent2.getComponent<Transform>();
			auto& vel1 = ent1.getComponent<Velocity>();
			auto& col2 = ent2.getComponent<AABB>();

			glm::vec2 direction(trans1.x - trans2.x, trans1.y - trans2.y);
			if (glm::length(direction) < 0.00001) {
				direction.x += rand() % 2 - 0.5f;
				direction.y += rand() % 2 - 0.5f;
			}

			direction = repulsionStrength * col1.mass*col2.mass * glm::normalize(direction);

			vel1.x += direction.x;
			vel1.y += direction.y;
		}
	}
}
