#pragma once

#include "../../src/Managers/Scene/SceneManager.hpp"

#include "../../src/ECS2/Systems/MovementS.hpp"
#include "../../src/ECS2/Systems/RenderingS.hpp"
#include "../../src/ECS2/Systems/CollisionS.hpp"
#include "../ECS/Systems/PlayerControllerS.hpp"
#include "../ECS/Systems/FollowS.hpp"


class Game1_scene1 : public Scene
{
public:
	Game1_scene1(Camera& camera, CollisionS& csys, RenderingS& rsys, MovementS& msys, PlayerControllerS& controllerSys);
	~Game1_scene1();
	virtual void update(float dt) override;
	virtual void draw(RenderingAPI* renderingAPI) override;
	virtual void input() override;
	
	CollisionS& csys;
	RenderingS& rsys;
	MovementS& msys;
	PlayerControllerS& controllerSys;
};