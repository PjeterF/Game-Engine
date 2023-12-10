#pragma once

#include "../../src/ECS/Systems/SystemBase.hpp"
#include "../../src/ECS/Systems/CollisionSystem.hpp"
#include "../../src/ECS/Systems/SpriteRenderingSystem.hpp"
#include "../../src/ECS/Components/BoxColliderC.hpp"
#include "../../src/ECS/Components/TransformC.hpp"
#include "../../src/ECS/Components/VelocityC.hpp"
#include "../../src/ECS/Components/BoxColliderC.hpp"
#include "../../src/ECS/Components/SpriteC.hpp"
#include "MarbleCollisionResolutionSystem.hpp"

#include "Spline.hpp"

#include <list>

struct MarbleTemplate
{
public:
	MarbleTemplate(float size, int tag, std::string textureFilepath);
	float size;
	int tag;
	std::string textureFilepath;
};

class RouteManagementSystem : public SystemBase
{
public:
	RouteManagementSystem(CollisionSystem* collisionSystem, SpriteRenderingSystem* srs, MarbleCollisionResolutionSystem* marblecollisionSystem, std::vector<glm::vec2> pathPoints);

	virtual void update(float dt) override;
	virtual void handleEvent(Event& event) override;

	void spawnRandomMarble();
	std::list<Ent*>::iterator deleteMarble(std::list<Ent*>::iterator it);

	float marbleSpeed = 1;
	int popThreshold = 3;
	int remainingMarblesToSpawn = 10;

	static std::vector<MarbleTemplate> marbleTemplates;
private:
	void moveRoutine(std::list<Ent*>::iterator startIterotor);
	int popSame(std::list<Ent*>::iterator it, int threshold);

	CollisionSystem* collisionSystem;
	SpriteRenderingSystem* spriteRenderingSystem;
	MarbleCollisionResolutionSystem* marblecollisionSystem;
	std::list<Ent*> marbles;
	std::vector<glm::vec2> pathPoints;
};