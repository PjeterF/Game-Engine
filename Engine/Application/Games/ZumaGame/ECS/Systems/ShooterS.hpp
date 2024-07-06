#pragma once

#include "../../src/ECS2/Systems/SysBase.hpp"
#include "../../src/ECS2/Systems/MovementS.hpp"
#include "../../src/ECS2/Systems/RenderingS.hpp"
#include "../../src/ECS2/Systems/CollisionS.hpp"
#include "../../src/ECS2/Systems/AnimationS.hpp"
#include "../../ECS/Systems/MarbleCollisionResolutionS.hpp"
#include "../../src/Graphics/GeneralRenderer.hpp"

#include "../Components/MarbleComponent.hpp"
#include "../Components/MarbleShooter.hpp"
#include"../../src/ECS2/Components/Transform.hpp"
#include"../../src/ECS2/Components/Velocity.hpp"

#include <vector>
#include <list>

class ShooterS : public SysBase
{
public:
	ShooterS(std::vector<std::string> marbleArchetypeFilepaths, MovementS& msys, RenderingS& rsys, CollisionS& csys, AnimationS& asys, MarbleCollisionResolutionS& mrsys);
	virtual void update(float dt) override;
	virtual void handleEvent(Event& event) override;
	
private:
	std::vector<std::string> marbleArchetypeFilepaths;
	MovementS& msys;
	RenderingS& rsys;
	CollisionS& csys;
	AnimationS& asys;
	MarbleCollisionResolutionS& mrsys;
};
