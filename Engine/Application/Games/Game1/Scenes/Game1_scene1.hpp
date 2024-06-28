#pragma once

#include "../../src/Managers/Scene/SceneManager.hpp"

#include "../../src/ECS2/Systems/MovementS.hpp"
#include "../../src/ECS2/Systems/RenderingS.hpp"
#include "../../src/ECS2/Systems/CollisionS.hpp"
#include "../../src/ECS2/Systems/CollisionRepulsionS.hpp"
#include "../ECS/Systems/PlayerControllerS.hpp"
#include "../ECS/Systems/FollowS.hpp"

#include <nlohmann/json.hpp>
#include <fstream>

class Game1_scene1 : public Scene
{
public:
	Game1_scene1(Camera& camera);
	virtual void initialize() override;
	virtual void update(float dt) override;
	virtual void draw(RenderingAPI* renderingAPI) override;
	virtual void input() override;

	int spawnEntity(std::string archetypeFilePath, glm::vec2 pos);

	int playerEntID=-1;
};