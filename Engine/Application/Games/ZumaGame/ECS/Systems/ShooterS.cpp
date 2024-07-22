#include "ShooterS.hpp"

#include "../../functions.hpp"
#include "../../src/ECS2/SystemsManager.hpp"

#include <filesystem>

ShooterS::ShooterS(std::vector<std::string>& marbleArchetypeFilepaths) : marbleArchetypeFilepaths(marbleArchetypeFilepaths)
{
	requiredComponents = {
		std::type_index(typeid(Transform)),
		std::type_index(typeid(MarbleShooter))
	};
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

			glm::vec2 offset =  transform.height * glm::vec2(cos(transform.rot), -sin(transform.rot));
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

	nlohmann::json jOut;
	for (auto ID : entities)
	{
		auto& transform = transformPool->get(ID);
		auto& shooterC = shooterCPool->get(ID);

		jOut["transform"]["pos"] = { transform.x, transform.y };
		jOut["transform"]["size"] = { transform.width, transform.height };

		jOut["shooterC"]["cooldown"] = shooterC.cooldown;
		jOut["shooterC"]["shotSpeed"] = shooterC.shotSpeed;

		if (spritePool->entityHasComponent[ID])
		{
			auto& sprite = spritePool->get(ID);

			jOut["Sprite"]["texture"] = sprite.getTexture()->getContents()->getFilepath();
			jOut["Sprite"]["sample"] = { sprite.textureSample.x, sprite.textureSample.y, sprite.textureSample.z, sprite.textureSample.w };
		}
	}

	return jOut;
}

nlohmann::json ShooterS::deSerialize()
{
	return nlohmann::json();
}
