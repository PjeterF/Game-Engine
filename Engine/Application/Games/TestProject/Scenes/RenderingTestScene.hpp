#pragma once

#include "../../src/Managers/Scene/SceneManager.hpp"
#include "../../src/ECS2/SystemsManager.hpp"
#include "../../src/Input/Windows/GLFWInputManager.hpp"

#include "../../src/ECS2/Systems/MovementS.hpp"
#include "../../src/ECS2/Systems/RenderingS.hpp"
#include "../../src/ECS2/Systems/CollisionS.hpp"
#include "../../src/ECS2/Systems/AnimationS.hpp"
#include "../../src/ECS2/Systems/CollisionRepulsionS.hpp"
#include "../../src/ECS2/Systems/ParticleS.hpp"
#include "../../src/ECS2/Systems/ElasticCollisionS.hpp"

#include "../../src/Utility/Utility.hpp"

#include <nlohmann/json.hpp>
#include <fstream>

class RenderingTestScene : public Scene
{
public:
	RenderingTestScene(Camera& camera);
	virtual void initialize() override;
	virtual void update(float dt) override;
	virtual void draw(RenderingAPI* renderingAPI) override;
	virtual void input() override;
private:
	std::chrono::steady_clock::time_point timeStart, timeEnd;
};