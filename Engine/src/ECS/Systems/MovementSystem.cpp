#include "MovementSystem.hpp"
#include "../Entity/EntManager.hpp"

MovementSystem::MovementSystem() : SystemBase(UNPAUSED, true, MovementSys)
{
	name = "MovementSys(" + std::to_string(ID) + ")";

	requiredComponents = { Transform, Velocity };
}

MovementSystem& MovementSystem::getInstance()
{
	static MovementSystem system;
	return system;
}

void MovementSystem::to_json(nlohmann::json& j) const
{
	j["type"] = type;
	j["entIDs"] = nlohmann::json::array();
	for (auto& ent : entities)
		j["entIDs"].push_back(ent.second->getID());
}

void MovementSystem::from_json(nlohmann::json& j)
{
	for (auto& entID : j["entIDs"])
	{
		Ent* ent = EntManager::getInstance().getEntity(entID);
		if (ent != nullptr)
			MovementSystem::getInstance().addEntity(ent);
	}
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
		transform->rotation = 57.2958 * atan(velocity->velocity.y / (velocity->velocity.x + 0.00001));
	}
}
