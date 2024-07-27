#include "ShooterS.hpp"

#include "../../functions.hpp"
#include "../../src/ECS2/SystemsManager.hpp"

#include "../../src/ECS2/Systems/RenderingS.hpp"

#include <filesystem>

ShooterS::ShooterS(std::vector<std::string>& marbleArchetypeFilepaths) : marbleArchetypeFilepaths(marbleArchetypeFilepaths)
{
	requiredComponents = {
		std::type_index(typeid(Transform)),
		std::type_index(typeid(MarbleShooter))
	};
}

ShooterS::~ShooterS()
{
	auto shooterCPool = ComponentPoolManager::getInstance().getPool<MarbleShooter>();
	for (auto ID : entities)
	{
		EntityManager::getInstance().deleteEntity(ID);
		if(shooterCPool->get(ID).currentShotID!=-1)
			EntityManager::getInstance().deleteEntity(shooterCPool->get(ID).currentShotID);
		if (shooterCPool->get(ID).nextShotID != -1)
			EntityManager::getInstance().deleteEntity(shooterCPool->get(ID).nextShotID);
	}
}

void ShooterS::update(float dt)
{
	auto transformPool = ComponentPoolManager::getInstance().getPool<Transform>();
	auto shooterCPool = ComponentPoolManager::getInstance().getPool<MarbleShooter>();

	for (auto ID : entities)
	{
		auto& transform = transformPool->get(ID);
		auto& shooterC = shooterCPool->get(ID);

		if (shooterC.counter != 0)
			shooterC.counter--;

		if (shooterC.currentShotID == -1)
		{
			shooterC.currentShotID = shooterC.nextShotID;
			shooterC.nextShotID = -1;
		}
		else
		{
			Transform& currTrans = transformPool->get(shooterC.currentShotID);

			glm::vec2 offset =  transform.height * glm::vec2(cos(transform.rot+1.57), -sin(transform.rot + 1.57));
			currTrans.x = transform.x + offset.x;
			currTrans.y = transform.y + offset.y;
		}

		if (shooterC.nextShotID == -1)
		{
			int marbleID = ZumaFn::spawnMarble(marbleArchetypeFilepaths[rand() % marbleArchetypeFilepaths.size()], { transform.x, transform.y }, "");

			SystemsManager::getInstance().getSystem<MovementS>()->addEntity(marbleID);
			SystemsManager::getInstance().getSystem<RenderingS>()->addEntity(marbleID);
			SystemsManager::getInstance().getSystem<CollisionS>()->addEntity(marbleID);
			SystemsManager::getInstance().getSystem<AnimationS>()->addEntity(marbleID);
			SystemsManager::getInstance().getSystem<MarbleCollisionResolutionS>()->addEntity(marbleID);

			shooterC.nextShotID = marbleID;
		}
		else
		{
			Transform& nextTrans = transformPool->get(shooterC.nextShotID);

			nextTrans.x = transform.x;
			nextTrans.y = transform.y;
		}
	}
}

void ShooterS::handleEvent(Event& event)
{
	SysBase::handleEvent(event);

	switch (event.getType())
	{
	case Event::Shoot:
	{
		auto transformPool = ComponentPoolManager::getInstance().getPool<Transform>();
		auto shooterCPool = ComponentPoolManager::getInstance().getPool<MarbleShooter>();
		auto velocityPool = ComponentPoolManager::getInstance().getPool<Velocity>();

		glm::vec2* targetPos = (glm::vec2*)event.getPayload();

		for (auto ID : entities)
		{
			auto& transform = transformPool->get(ID);
			auto& shooterC = shooterCPool->get(ID);

			if (shooterC.currentShotID == -1 || shooterC.counter!=0)
				continue;

			glm::vec2 dir = { targetPos->x - transform.x, targetPos->y - transform.y };
			if (glm::length(dir) < 0.001)
				dir = { 0, 1 };
			else
				dir = glm::normalize(dir);

			Velocity& shotVel = velocityPool->get(shooterC.currentShotID);
			Transform& shotTrans = transformPool->get(shooterC.currentShotID);

			shotTrans.x = transform.x + transform.height * dir.x;
			shotTrans.y = transform.y + transform.height * dir.y;

			shotVel.x = shooterC.shotSpeed * dir.x;
			shotVel.y = shooterC.shotSpeed * dir.y;

			shooterC.currentShotID = -1;
			shooterC.counter = shooterC.cooldown;

			transform.rot = atan2(-dir.y, dir.x)-3.14/2;
		}
	}
	case Event::UpdateMarbleArchetypes:
	{
		namespace fs = std::filesystem;

		fs::path dirPath(archetypesDirectoryFilepath);

		marbleArchetypeFilepaths.clear();
		for (auto& entry : fs::directory_iterator(dirPath))
		{
			std::string fileName = entry.path().string();
			marbleArchetypeFilepaths.push_back(fileName);
		}
	}
	break;
	}
}

nlohmann::json ShooterS::serialize()
{
	auto transformPool = ComponentPoolManager::getInstance().getPool<Transform>();
	auto shooterCPool = ComponentPoolManager::getInstance().getPool<MarbleShooter>();
	auto spritePool = ComponentPoolManager::getInstance().getPool<Sprite>();
	auto layerPool = ComponentPoolManager::getInstance().getPool<RenderingLayer>();

	nlohmann::json jOut = nlohmann::json::array();
	for (auto ID : entities)
	{
		nlohmann::json jEnt;
		jEnt["Components"] = nlohmann::json::array();

		jEnt["Components"].push_back(shooterCPool->get(ID).serialize());
		jEnt["Components"].push_back(transformPool->get(ID).serialize());
		if (spritePool->entityHasComponent[ID])
			jEnt["Components"].push_back(spritePool->get(ID).serialize());
		if (layerPool->entityHasComponent[ID])
			jEnt["Components"].push_back(layerPool->get(ID).serialize());

		jOut.push_back(jEnt);
	}

	return jOut;
}

void ShooterS::deSerialize(nlohmann::json j)
{
	auto shooterCPool = ComponentPoolManager::getInstance().getPool<MarbleShooter>();
	for (auto ID : entities)
	{
		auto& shooterC = shooterCPool->get(ID);
		if(shooterC.currentShotID != -1)
			EntityManager::getInstance().deleteEntity(shooterC.currentShotID);
		if (shooterC.nextShotID != -1)
			EntityManager::getInstance().deleteEntity(shooterC.nextShotID);

		EntityManager::getInstance().deleteEntity(ID);
	}
	EntityManager::getInstance().update();

	for (int i = 0; i < j.size(); i++)
	{
		Entity newEnt = EntityManager::getInstance().createEntity();

		for (int i2 = 0; i2 < j[i]["Components"].size(); i2++)
		{
			int type = j[i]["Components"][i2]["type"];
			switch (type)
			{
			case CBase::Transform:
			{
				Transform comp;
				comp.deSerialize(j[i]["Components"][i2]);
				newEnt.addComponent<Transform>(comp);
			}
			break;
			case CBase::MarbleShooter:
			{
				MarbleShooter comp;
				comp.deSerialize(j[i]["Components"][i2]);
				newEnt.addComponent<MarbleShooter>(comp);
			}
			break;
			case CBase::Sprite:
			{
				Sprite comp;
				comp.deSerialize(j[i]["Components"][i2]);
				newEnt.addComponent<Sprite>(comp);
			}
			break;
			case CBase::RenderingLayer:
			{
				RenderingLayer comp;
				comp.deSerialize(j[i]["Components"][i2]);
				newEnt.addComponent<RenderingLayer>(comp);
			}
			break;
			}
		}

		newEnt.addComponent<Velocity>(Velocity());
		SystemsManager::getInstance().getSystem<RenderingS>()->addEntity(newEnt.getID());
		this->addEntity(newEnt.getID());
	}
}
