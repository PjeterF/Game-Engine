#pragma once

#include "../../src/Managers/SceneManager.hpp"

#include "../../src/ECS2/Systems/MovementS.hpp"
#include "../../src/ECS2/Systems/RenderingS.hpp"
#include "../../src/ECS2/Systems/CollisionS.hpp"
#include "../../src/ECS2/Systems/UserControllerS.hpp"

class Game1_scene1 : public Scene
{
public:
	Game1_scene1();
	~Game1_scene1();
	virtual void update(float dt) override;
	virtual void draw(RenderingAPI* renderingAPI) override;
	virtual void input() override;
private:
	MovementS* msys;
	UserControllerS* inputsys;
};