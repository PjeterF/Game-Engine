#include "MovementSystem.hpp"

MovementSystem::MovementSystem()
{
	name = "MovementSys(" + std::to_string(ID) + ")";

	requiredComponents = { Transform, Velocity };
}

void MovementSystem::handleEvent(Event& event)
{
	SystemBase::handleEvent(event);

	switch (event.getType())
	{
	case Event::MarbleInsertion:
	{
		Ent* entity = (Ent*)event.getPayload();
		removeEntity(entity->getID());
	}
	break;
	default:
		break;
	}
}

void MovementSystem::update(float dt)
{
	for (auto& entity : entities)
	{
		auto transform = (TransformC*)entity.second->getComponent(CType::Transform);
		auto velocity = (VelocityC*)entity.second->getComponent(CType::Velocity);

		transform->position = transform->position + velocity->velocity;
	}
}
