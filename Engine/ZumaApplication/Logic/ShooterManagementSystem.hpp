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
	ShooterManagementSystem();
	virtual void update(float dt);

	virtual void to_json(nlohmann::json& j) const override;
	virtual void from_json(nlohmann::json& j) override;

	virtual bool addEntity(Ent* entity) override;
	virtual void handleEvent(Event& event) override;
private:
	Ent* generateShot(MarbleTemplate mTemplate, glm::vec2 pos);
	float lookAtAngle(glm::vec2 target, glm::vec2 position);
};