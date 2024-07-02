#include "PlayerControllerS.hpp"

#include "../../src/ECS2/Components/Transform.hpp"
#include "../../src/ECS2/Components/Velocity.hpp"

#include <cmath>
#include <glm/glm.hpp>

PlayerControllerS::PlayerControllerS(InputManager& input) : input(input)
{
	requiredComponents = { std::type_index(typeid(Transform)), std::type_index(typeid(Velocity)) };
}

void PlayerControllerS::update(float dt)
{
	for (auto entID : entities)
	{
		Entity ent(entID);
		auto& trans = ent.getComponent<Transform>();
		auto& vel = ent.getComponent<Velocity>();

		if (input.keyDown[ZE_KEY_W]) {
			vel.y = vel.y + velocityIncrease;
		}
		if (input.keyDown[ZE_KEY_S]) {
			vel.y = vel.y - velocityIncrease;
		}
		if (input.keyDown[ZE_KEY_D]) {
			vel.x = vel.x + velocityIncrease;
		}
		if (input.keyDown[ZE_KEY_A]) {
			vel.x = vel.x - velocityIncrease;
		}
		if (input.keyDown[ZE_KEY_SPACE]) {
			glm::vec2 dir = { vel.x, vel.y };
			if (glm::length(dir) != 0)
			{
				dir = dashSpeed * glm::normalize(dir);
				vel.x = dir.x;
				vel.y = dir.y;
			}
		}
	}
}
