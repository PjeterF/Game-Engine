#include "UserControllerS.hpp"

#include "../Components/Transform.hpp"
#include "../Components/Velocity.hpp"

#include <glm/glm.hpp>

UserControllerS::UserControllerS(InputManager* input) : input(input)
{
	requiredComponents = { std::type_index(typeid(Transform)), std::type_index(typeid(Velocity)) };
}

void UserControllerS::initialize(InputManager* input)
{
	instanceImp(input);
}

UserControllerS& UserControllerS::getInstance()
{
	return instanceImp(nullptr);
}

void UserControllerS::update(float dt)
{
	enum Direction {up=1<<0, down=1<<1, left=1<<2, right=1<<3};

	float maxVelocity = 5;
	float velIncrease = 0.5;
	for (auto entID : entities)
	{
		Entity ent(entID);
		Transform& trans = ent.getComponent<Transform>();
		Velocity& vel = ent.getComponent<Velocity>();

		if (input->keyDown[ZE_KEY_W])
		{
			vel.y = vel.y + velIncrease;
		}
		if (input->keyDown[ZE_KEY_S])
		{
			vel.y = vel.y - velIncrease;
		}
		if (input->keyDown[ZE_KEY_A])
		{
			vel.x = vel.x - velIncrease;
		}
		if (input->keyDown[ZE_KEY_D])
		{
			vel.x = vel.x + velIncrease;
		}
		glm::vec2 velVec(vel.x, vel.y);
		if (glm::length(velVec) > maxVelocity)
		{
			velVec = glm::normalize(velVec);
			vel.x = velVec.x;
			vel.y = velVec.y;
		}
	}
}

UserControllerS& UserControllerS::instanceImp(InputManager* input)
{
	static UserControllerS instance(input);
	return instance;
}
