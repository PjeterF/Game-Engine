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
#include"../../src/ECS2/Components/Sprite.hpp"

#include <vector>
#include <list>
#include <nlohmann/json.hpp>

class ShooterS : public SysBase
{
public:
	ShooterS(std::vector<std::string>& marbleArchetypeFilepaths);
	virtual void update(float dt) override;
	virtual void handleEvent(Event& event) override;

	nlohmann::json serialize();
	nlohmann::json deSerialize();
	
private:
	std::string archetypesDirectoryFilepath = "Application/Games/ZumaGame/MarbleArchetypes";
	std::vector<std::string> marbleArchetypeFilepaths;
};
