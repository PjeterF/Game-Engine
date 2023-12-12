#pragma once

#include "../../src/ECS/Systems/SystemBase.hpp"
#include "../../src/ECS/Systems/CollisionSystem.hpp"
#include "../../src/ECS/Systems/SpriteRenderingSystem.hpp"
#include "../../src/ECS/Systems/MovementSystem.hpp"
#include "../../src/ECS/Components/BoxColliderC.hpp"
#include "../../src/ECS/Components/TransformC.hpp"
#include "../../src/ECS/Components/VelocityC.hpp"
#include "../../src/ECS/Components/BoxColliderC.hpp"
#include "../../src/ECS/Components/SpriteC.hpp"
#include "RouteInfoC.hpp"
#include "ShooterInfoC.hpp"
#include "RouteManagementSystem.hpp"
#include "MarbleCollisionResolutionSystem.hpp"

#include "../../src/Events/EventManager.hpp"

class ShooterManagementSystem : public SystemBase
{
public:
	ShooterManagementSystem(CollisionSystem* collisionSystem, AnimatedSpriteSystem* srs, MovementSystem* movementSystem);
	virtual void update(float dt);
	virtual bool addEntity(Ent* entity) override;
	virtual void handleEvent(Event& event) override;

	float shotVelocity = 5;
private:
	void shoot(glm::vec2 target, glm::vec2 startPos, MarbleTemplate mTemplate);
	Ent* generateShot(MarbleTemplate mTemplate, glm::vec2 pos);
	float lookAtAngle(glm::vec2 target, glm::vec2 position);

	int cooldownTime = 30;
	int counter = 0;
	bool onCooldown = false;

	CollisionSystem* collisionSystem;
	AnimatedSpriteSystem* spriteRenderingSystem;
	MovementSystem* movementSystem;
};