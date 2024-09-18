#pragma once

#include "../../src/Managers/Scene/SceneManager.hpp"

#include "../../src/ECS2/Systems/MovementS.hpp"
#include "../../src/ECS2/Systems/AnimationS.hpp"
#include "../../src/ECS2/Systems/RenderingS.hpp"
#include "../../src/ECS2/Systems/CollisionS.hpp"
#include "../../src/ECS2/Systems/CollisionRepulsionS.hpp"
#include "../../src/ECS2/Systems/ParticleS.hpp"
#include "../ECS/Systems/ShooterS.hpp"
#include "../ECS/Systems/CounterKillerS.hpp"
#include "../ECS/Systems/RouteS.hpp"

#include "../../Application/Tilemap/Tilemap.hpp"

#include "../UI/MarbleEditor.hpp"
#include "../UI/ZumaMenu.hpp"
#include "../UI/EntitiesMenu.hpp"
#include "../UI/PropertiesMenu.hpp"
#include "../UI/TextureManager.hpp"

#include "../../src/Graphics/ParticleEmitter.hpp"

#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>

class GeneralZumaScene : public Scene, public EventObserver
{
public:
	GeneralZumaScene(Camera& camera);
	virtual void handleEvent(Event& event) override;

	virtual void initialize() override;
	virtual void update(float dt) override;
	virtual void draw(RenderingAPI* renderingAPI) override;
	virtual void input() override;

	void serialize(std::string filepath);
	void deSerialize(std::string filepath);
private:
	void updateMarbleArchetypes(std::string folderpath);

	ParticeEmitter* emitter;

	std::vector<std::string> marbleArchetypeFilepaths;
	std::string selectedRoute = "";
	int ctrlPtIdx = -1;
	bool movingPt = false;
	std::vector<UIWindow*> UIElements;

	bool paused = false;

	bool showCollisionGrid = false;
	float cellSize = 30;
	float gridLineWidth = 1;

	bool showDebug = false;
};