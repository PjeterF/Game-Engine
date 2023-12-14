#pragma once

#include "../../src/ECS/Systems/SystemBase.hpp"
#include "../../src/ECS/Systems/CollisionSystem.hpp"
#include "../../src/ECS/Systems/AnimatedSpriteSystem.hpp"
#include "../../src/ECS/Components/BoxColliderC.hpp"
#include "../../src/ECS/Components/TransformC.hpp"
#include "../../src/ECS/Components/VelocityC.hpp"
#include "../../src/ECS/Components/BoxColliderC.hpp"
#include "../../src/ECS/Components/AnimatedSpriteC.hpp"
#include "../../src/ECS/Components/RenderingLayerC.hpp"
#include "MarbleCollisionResolutionSystem.hpp"

#include "Spline.hpp"

#include <list>

struct MarbleTemplate
{
public:
	MarbleTemplate(float size, int tag, std::string textureFilepath, std::vector<TextureDivision> divisions, int frameDuration);
	float size;
	int tag;
	std::string textureFilepath;
	std::vector<TextureDivision> divisions;
	int frameDuration;
};

class RouteManagementSystem : public SystemBase
{
public:
	RouteManagementSystem(CollisionSystem* collisionSystem, AnimatedSpriteSystem* srs, MarbleCollisionResolutionSystem* marblecollisionSystem, std::vector<glm::vec2> pathPoints);

	virtual void update(float dt) override;
	virtual void handleEvent(Event& event) override;

	void spawnRandomMarble();
	void setLayer(int target);

	float marbleSpeed = 1;
	int popThreshold = 3;
	int remainingMarblesToSpawn = 1000;

	static std::vector<MarbleTemplate> marbleTemplates;
private:
	void moveRoutine(std::list<Ent*>::iterator startIterotor);
	int popSame(std::list<Ent*>::iterator it, int threshold);

	int layer = 0;

	CollisionSystem* collisionSystem;
	AnimatedSpriteSystem* spriteRenderingSystem;
	MarbleCollisionResolutionSystem* marblecollisionSystem;
	std::list<Ent*> marbles;
	std::vector<glm::vec2> pathPoints;
};