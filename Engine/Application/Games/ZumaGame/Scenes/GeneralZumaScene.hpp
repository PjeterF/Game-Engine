#pragma once

#include "../../src/Managers/Scene/SceneManager.hpp"

#include "../../src/ECS2/Systems/MovementS.hpp"
#include "../../src/ECS2/Systems/AnimationS.hpp"
#include "../../src/ECS2/Systems/RenderingS.hpp"
#include "../../src/ECS2/Systems/CollisionS.hpp"
#include "../../src/ECS2/Systems/CollisionRepulsionS.hpp"
#include "../ECS/Systems/ShooterS.hpp"

#include "../../Application/Tilemap/Tilemap.hpp"

#include "../UI/MarbleEditor.hpp"
#include "../UI/ZumaMenu.hpp"

#include <nlohmann/json.hpp>
#include <fstream>

class GeneralZumaScene : public Scene
{
public:
	GeneralZumaScene(Camera& camera);
	virtual void initialize() override;
	virtual void update(float dt) override;
	virtual void draw(RenderingAPI* renderingAPI) override;
	virtual void input() override;
private:
	std::string selectedRoute = "";
	int ctrlPtIdx = -1;
	bool movingPt = false;
	std::vector<UIWindow*> UIElements;
};