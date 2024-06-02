#include "MovementSystem.hpp"
#include "../Entity/EntManager.hpp"

MovementSystem::MovementSystem() : SystemBase(UNPAUSED, true, MovementSys)
{
	name = "MovementSys(" + std::to_string(ID) + ")";

	requiredComponents = { Transform_, Velocity_ };
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
		j["entIDs"].push_back(ent->getID());
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
		auto transC = (TransformC*)entity->getComponent(CType::Transform_);
		auto velC = (VelocityC*)entity->getComponent(CType::Velocity_);

		transC->position = transC->position + velC->velocity;
		transC->rotation = 57.2958 * atan(velC->velocity.y / (velC->velocity.x + 0.00001));
	}
}
