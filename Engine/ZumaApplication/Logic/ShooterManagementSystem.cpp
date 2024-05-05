#include "ShooterManagementSystem.hpp"
#include "../../src/ECS/Entity/EntManager.hpp"
#include "../../src/ECS/Systems/LayeredRenderingSystem.hpp"

ShooterManagementSystem::ShooterManagementSystem() : SystemBase(UNPAUSED, true, ShooterManagementSys)
{
	EventManager::getInstance().addObserver(this, UI);

	name = "ShooterSys(" + std::to_string(ID) + ")";

	requiredComponents = { Transform, ShooterInfo, RenderingLayer };
	EventManager::getInstance().addObserver(this, ObsBin::Defualt);
}

ShooterManagementSystem& ShooterManagementSystem::getInstance()
{
	static ShooterManagementSystem sys;
	return sys;
}

void ShooterManagementSystem::update(float dt)
{
	for (auto& entity : entities)
	{
		auto shooterComponent = (ShooterC*)entity->getComponent(ShooterInfo);
		if (shooterComponent->counter != 0)
			shooterComponent->counter--;
	}
}

void ShooterManagementSystem::to_json(nlohmann::json& j) const
{
	j["type"] = type;
	j["entIDs"] = nlohmann::json::array();
	for (auto& ent : entities)
		j["entIDs"].push_back(ent->getID());
}

void ShooterManagementSystem::from_json(nlohmann::json& j)
{
	for (auto& entID : j["entIDs"])
	{
		Ent* ent = EntManager::getInstance().getEntity(entID);
		if (ent != nullptr)
			this->addEntity(ent);
	}
}

bool ShooterManagementSystem::addEntity(Ent* entity)
{
	if (!SystemBase::addEntity(entity))
		return false;

	auto transform = (TransformC*)entity->getComponent(Transform);
	auto shooterComponent = (ShooterC*)entity->getComponent(ShooterInfo);

	if (shooterComponent->currShotEntID == -1)
	{
		Ent* newShot = generateShot(RouteManagementSystem::marbleTemplates[rand() % RouteManagementSystem::marbleTemplates.size()], transform->position);
		shooterComponent->currShotEntID = newShot->getID();
	}

	if (shooterComponent->nextShotEntID == -1)
	{
		Ent* newShot = generateShot(RouteManagementSystem::marbleTemplates[rand() % RouteManagementSystem::marbleTemplates.size()], transform->position);
		shooterComponent->nextShotEntID = newShot->getID();
	}
}

void ShooterManagementSystem::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::Shoot:
	{
		auto targetPos = (glm::vec2*)event.getPayload();
		for (auto& entity : entities)
		{
			auto shooterComponent = (ShooterC*)entity->getComponent(ShooterInfo);
			if (shooterComponent->counter != 0)
				continue;

			auto transform = (TransformC*)entity->getComponent(Transform);
			auto layer = (RenderingLayerC*)entity->getComponent(RenderingLayer);

			Ent* shotFired = EntManager::getInstance().getEntity(shooterComponent->currShotEntID);
			auto shotFired_Velocity = (VelocityC*)shotFired->getComponent(Velocity);
			auto shotFired_Transform = (TransformC*)shotFired->getComponent(Transform);
			auto shotFired_Layer = (RenderingLayerC*)shotFired->getComponent(RenderingLayer);
			shotFired_Layer->layer = layer->layer;

			transform->rotation = lookAtAngle(*targetPos, transform->position);
			shotFired_Transform->position = transform->position + transform->size * glm::normalize(*targetPos - transform->position);
			shotFired_Velocity->velocity = shooterComponent->shotSpeed * glm::normalize(*targetPos - shotFired_Transform->position);

			Ent* nextShot = EntManager::getInstance().getEntity(shooterComponent->nextShotEntID);
			auto nextShotTransform = (TransformC*)nextShot->getComponent(Transform);
			nextShotTransform->position = transform->position + transform->size * glm::normalize(*targetPos - transform->position);

			shooterComponent->currShotEntID = shooterComponent->nextShotEntID;
			Ent* newShot = generateShot(RouteManagementSystem::marbleTemplates[rand() % RouteManagementSystem::marbleTemplates.size()], transform->position);
			shooterComponent->nextShotEntID = newShot->getID();

			shooterComponent->counter = shooterComponent->cooldownTime;
		}
	}
	break;
	case Event::MarbleInsertion:
	{
		Ent* entity = (Ent*)event.getPayload();
		removeEntity(entity->getID());
	}
	break;
	case Event::MouseMove:
	{
		/*auto targetPos = (glm::vec2*)event.getPayload();
		for (auto& entity : entities)
		{
			auto transform = (TransformC*)entity->getComponent(Transform);

			transform->rotation = lookAtAngle(*targetPos, transform->position);
		}*/
	}
	break;
	default:
		break;
	}

	SystemBase::handleEvent(event);
}

Ent* ShooterManagementSystem::generateShot(MarbleTemplate mTemplate, glm::vec2 pos)
{
	Ent* newMarble = EntManager::getInstance().createEntity();
	newMarble->addComponent(new BoxColliderC(pos.x, pos.y, 10, 10, newMarble));
	newMarble->addComponent(new TransformC(pos, { mTemplate.size, mTemplate.size }, 0));
	newMarble->addComponent(new VelocityC({ 0, 0 }));
	newMarble->addComponent(new RouteInfoC(mTemplate.tag, -1));
	newMarble->addComponent(new RenderingLayerC(0));
	newMarble->addComponent(new AnimatedSpriteC(ResourceManager::getInstance().getResource<Texture>(mTemplate.textureFilepath), mTemplate.divisions, mTemplate.frameDuration));

	CollisionSystem::getInstance().addEntity(newMarble);
	LayeredRenderingSystem::getInstance().addEntity(newMarble);
	MovementSystem::getInstance().addEntity(newMarble);
	
	return newMarble;
}

float ShooterManagementSystem::lookAtAngle(glm::vec2 target, glm::vec2 position)
{
	glm::vec2 vector = target - position;
	vector = glm::normalize(vector);
	float angle;

	if (target.x >= position.x && target.y >= position.y)
	{
		angle = atan(vector.y / vector.x) * 57.2958 + 90;
	}
	else if (target.x < position.x && target.y >= position.y)
	{
		angle = 180 - atan(-vector.y / vector.x) * 57.2958 + 90;
	}
	else if (target.x < position.x && target.y < position.y)
	{
		angle = 180 + atan(vector.y / vector.x) * 57.2958 + 90;
	}
	else if (target.x >= position.x && target.y < position.y)
	{
		angle = 360 - atan(-vector.y / vector.x) * 57.2958 + 90;
	}

	return angle;
}
