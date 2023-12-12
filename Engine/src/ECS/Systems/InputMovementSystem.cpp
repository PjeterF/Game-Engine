#include "InputMovementSystem.hpp"

#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>

InputMovementSystem::InputMovementSystem(InputManager* inputManager) : SystemBase(UNPAUSED)
{
	name = "InputMovementSys(" + std::to_string(ID) + ")";

	requiredComponents = { Velocity };
	this->input = inputManager;
}

void InputMovementSystem::update(float dt)
{
	float maxSpeed = 5;
	float step = 0.1;

	for (auto& entity : entities)
	{
		auto velocity = (VelocityC*)entity.second->getComponent(CType::Velocity);

		glm::vec2 acceleration = { 0, 0 };

		if (input->isKeyPressed(GLFW_KEY_A))
		{
			acceleration.x = -step;
		}
		else if (input->isKeyPressed(GLFW_KEY_D))
		{
			acceleration.x = step;
		}

		if (input->isKeyPressed(GLFW_KEY_S))
		{
			acceleration.y = -step;
		}
		else if (input->isKeyPressed(GLFW_KEY_W))
		{
			acceleration.y = step;
		}

		if(acceleration.x != 0 || acceleration.y != 0)
			velocity->velocity = velocity->velocity + acceleration;

		if (abs(velocity->velocity.x) > maxSpeed || abs(velocity->velocity.y) > maxSpeed)
		{
			velocity->velocity = maxSpeed * glm::normalize(velocity->velocity);
		}
	}
}
