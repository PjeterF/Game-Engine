#include "GeneralZumaScene.hpp"

#include "../../src/Input/Windows/GLFWInputManager.hpp"

#include "../ECS/Systems/MarbleCollisionResolutionS.hpp"
#include "../../src/ECS2/SystemsManager.hpp"

#include <fstream>

#include "../../src/ECS2/Components/Transform.hpp"

GeneralZumaScene::GeneralZumaScene(Camera& camera) : Scene(camera), EventObserver(ApplicationBin)
{
}

void GeneralZumaScene::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::TogglePause:
		paused = !paused;
	break;
	case Event::UpdateMarbleArchetypes:
	{
		updateMarbleArchetypes("Application/Games/ZumaGame/MarbleArchetypes");
	}
	break;
	}
}

void GeneralZumaScene::initialize()
{
	glm::ivec2 windowDimensions = GLFWInputManager::getInstance().getWindowDimensions();

	float UIElementWidth = 300;

	UIElements.push_back(new TextureManager("Textures Manager", 0, 0, UIElementWidth, windowDimensions.y/2));
	UIElements.push_back(new ZumaMenu("Zuma Menu", 0, windowDimensions.y / 2, UIElementWidth, windowDimensions.y / 2, "Application/Games/ZumaGame/MarbleArchetypes"));
	UIElements.push_back(new EntitiesMenu("Entities", windowDimensions.x-UIElementWidth, 0, UIElementWidth, windowDimensions.y / 2));
	UIElements.push_back(new PropertiesMenu("Properties", windowDimensions.x-UIElementWidth, windowDimensions.y / 2, UIElementWidth, windowDimensions.y / 2));

	updateMarbleArchetypes("Application/Games/ZumaGame/MarbleArchetypes");

	std::vector<glm::vec2> ctrlPts = {
		{100, 100}, 
		{200, 400},
		{300, 500}, 
		{700, 400},
		{1100, 300},
		{1200, 500},
		{1300, 100}
	};

	//SystemsManager::getInstance().addSystem<RouteS>(new RouteS("R1", ctrlPts, 10, 100, 10), "R1");
	SystemsManager::getInstance().addSystem<MarbleCollisionResolutionS>(new MarbleCollisionResolutionS());
	SystemsManager::getInstance().addSystem<CollisionS>(new CollisionS(30));
	SystemsManager::getInstance().addSystem<MovementS>(new MovementS());
	SystemsManager::getInstance().addSystem<AnimationS>(new AnimationS());
	SystemsManager::getInstance().addSystem<CollisionRepulsionS>(new CollisionRepulsionS());

	SystemsManager::getInstance().addSystem<ShooterS>(new ShooterS(marbleArchetypeFilepaths));

	auto test = (ResourceManager::getInstance().getResource<Texture>("Application/Games/ZumaGame/Textures/frog.png"));

	/*Entity shooter = EntityManager::getInstance().createEntity();
	shooter.addComponent<Transform>(Transform(1000, 1000, 100, 100));
	shooter.addComponent<RenderingLayer>(RenderingLayer());
	shooter.addComponent<MarbleShooter>(MarbleShooter(400, 30));
	shooter.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>("Application/Games/ZumaGame/Textures/frog.png")));

	SystemsManager::getInstance().getSystem<RenderingS>()->addEntity(shooter.getID());
	SystemsManager::getInstance().getSystem<ShooterS>()->addEntity(shooter.getID());*/

	emitter = new ParticeEmitter(0, 2000, 10000);
	emitter->defaultProperties.xPosVar = glm::vec2(-50, 50);
	emitter->defaultProperties.yPosVar = glm::vec2(0, 25);
	emitter->defaultProperties.yVelVar = glm::vec2(1, 2);
	emitter->defaultProperties.xVelVar = glm::vec2(-0.5, 0.5);
	emitter->defaultProperties.startColour = glm::vec4(1, 0.9, 0.3, 1);
	emitter->defaultProperties.endColour = glm::vec4(0.8, 0.1, 0.1, 0.5);
	emitter->defaultProperties.startSize = 15;
	emitter->defaultProperties.endSize = 0;
	emitter->defaultProperties.velocityDecay = 0.9999;
	emitter->defaultProperties.particleLifetime = { 100, 300 };

	SystemsManager::getInstance().addSystem<ParticleS>(new ParticleS());
	SystemsManager::getInstance().addSystem<CounterKillerS>(new CounterKillerS());
}

void GeneralZumaScene::update(float dt)
{
	EntityManager::getInstance().update();
	SystemsManager::getInstance().getSystem<CollisionS>()->lateUpdate(dt);
	SystemsManager::getInstance().getSystem<CollisionS>()->update(dt);

	SystemsManager::getInstance().getSystem<MarbleCollisionResolutionS>()->update(dt);
	for (auto& item : SystemsManager::getInstance().getSystemBin<RouteS>())
	{
		((RouteS*)item.second)->spawnMarbleIfPossible(
			marbleArchetypeFilepaths[rand() % marbleArchetypeFilepaths.size()]);
	}

	SystemsManager::getInstance().getSystem<ParticleS>()->update(dt);
	SystemsManager::getInstance().getSystem<CounterKillerS>()->update(dt);

	SystemsManager::getInstance().getSystem<ShooterS>()->update(dt);
	for (auto& item : SystemsManager::getInstance().getSystemBin<RouteS>())
	{
		RouteS* route = (RouteS*)item.second;
		route->update(dt);
	}

	SystemsManager::getInstance().getSystem<AnimationS>()->update(dt);
	SystemsManager::getInstance().getSystem<MovementS>()->update(dt);

	for(int i=0;i<10;i++)
		emitter->emit();
	emitter->applyForceInverseToSize(0.1, 0.1);
	emitter->update();
}

void GeneralZumaScene::draw(RenderingAPI* renderingAPI)
{
	//SystemsManager::getInstance().getSystem<ParticleS>()->draw(renderingAPI);
	
	SystemsManager::getInstance().getSystem<RenderingS>()->update(0);

	for (auto& element : UIElements)
		element->render();

	emitter->draw(renderingAPI);

	if (paused)
	{
		for (auto& item : SystemsManager::getInstance().getSystemBin<RouteS>())
		{
			((RouteS*)item.second)->draw(renderingAPI);
		}
	}

	if (paused)
	{
		renderingAPI->drawLine({ 0, -10000 }, { 0, 10000 }, 5, { 0.7, 0.7, 0.7 });
		renderingAPI->drawLine({ -10000, 0 }, { 10000, 0 }, 5, { 0.7, 0.7, 0.7 });
	}

	if (showCollisionGrid)
	{
		SystemsManager::getInstance().getSystem<CollisionS>()->drawGrid(*renderingAPI, camera, gridLineWidth, { 1, 1, 1 ,1 }, true);
		SystemsManager::getInstance().getSystem<CollisionS>()->drawCellsWithColliders(*renderingAPI, camera, gridLineWidth, { 1, 0, 0 ,1 }, true);
	}

	if (showDebug)
	{
		ImGui::Begin("Debug");

		ImGui::InputFloat("Collision grid cell size", &SystemsManager::getInstance().getSystem<CollisionS>()->cellSize);
		ImGui::InputFloat("Collision grid line width", &gridLineWidth);
		ImGui::Checkbox("Show collision grid", &showCollisionGrid);

		ImGui::End();
	}
	
}

void GeneralZumaScene::input()
{
	auto& input = GLFWInputManager::getInstance();

	auto cursorPos = (input.getCursorPos() / camera.getZoom() + camera.getPosition());

	float scrollRegionWidth = 0.01f;
	float rate = 10.0f / camera.getZoom();
	auto normalizedCursorPos = input.getNormalizedCursorPos();
	auto cameraPosition = camera.getPosition();
	float wheel = input.mouseWheel();
	float zoomRate = 0.02;

	if (normalizedCursorPos.x < scrollRegionWidth)
		camera.setPosition(cameraPosition.x - rate, cameraPosition.y);
	if (normalizedCursorPos.x > 1.0f - scrollRegionWidth)
		camera.setPosition(cameraPosition.x + rate, cameraPosition.y);
	if (normalizedCursorPos.y < scrollRegionWidth)
		camera.setPosition(cameraPosition.x, cameraPosition.y - rate);
	if (normalizedCursorPos.y > 1.0f - scrollRegionWidth)
		camera.setPosition(cameraPosition.x, cameraPosition.y + rate);
	if (wheel != 0)
		camera.changeZoom(wheel * zoomRate);

	if (input.keyReleased[ZE_KEY_P])
		showDebug = !showDebug;

	for (auto& element : UIElements)
	{
		if (element->isHovered())
		{
			input.update();
			return;
		}
	}

	if (input.keyClicked[ZE_KEY_SPACE])
		EventManager::getInstance().notify(Event(Event::TogglePause, nullptr), ApplicationBin);
	if (input.keyClicked[ZE_KEY_Q])
	{
		auto it = SystemsManager::getInstance().getSystemBin<RouteS>().find(selectedRoute);
		if(it != SystemsManager::getInstance().getSystemBin<RouteS>().end())
			SystemsManager::getInstance().getSystem<RouteS>(selectedRoute)->addSegment(cursorPos);
	}
		
	if (input.keyClicked[ZE_KEY_E])
	{
		auto it = SystemsManager::getInstance().getSystemBin<RouteS>().find(selectedRoute);
		if (it != SystemsManager::getInstance().getSystemBin<RouteS>().end())
			SystemsManager::getInstance().getSystem<RouteS>(selectedRoute)->removeLastSegment();
	}
	
	if (input.mouseKeyClicked[ZE_MOUSE_BUTTON_1])
		EventManager::getInstance().notify(Event(Event::Shoot, &cursorPos), ECS2);

	if (movingPt)
	{
		auto route = SystemsManager::getInstance().getSystem<RouteS>(selectedRoute);
		route->setControlPointPos(ctrlPtIdx, cursorPos);

		if (input.mouseKeyClicked[ZE_MOUSE_BUTTON_2])
		{
			ctrlPtIdx = -1;
			movingPt = false;
		}
	}
	else
	{
		if (input.mouseKeyClicked[ZE_MOUSE_BUTTON_2])
		{
			for (auto& route : SystemsManager::getInstance().getSystemBin<RouteS>())
			{
				int result = ((RouteS*)route.second)->ctrlPointIntersection(cursorPos);
				if (result == -1)
					continue;

				selectedRoute = route.first;
				EventManager::getInstance().notify(Event(Event::RouteSelection, &selectedRoute), UI);
				EventManager::getInstance().notify(Event(Event::RouteSelection, &selectedRoute), ECS2);

				ctrlPtIdx = result;
				movingPt = true;
				break;
			}
		}
	}
	SceneManager::getInstance();
	input.update();
}

void GeneralZumaScene::serialize(std::string filepath)
{
	nlohmann::json jOut;

	jOut["Routes"] = nlohmann::json::array();
	for (auto& route : SystemsManager::getInstance().getSystemBin<RouteS>())
	{
		nlohmann::json jRoute = ((RouteS*)(route.second))->serialize();
		jOut["Routes"].push_back(jRoute);
	}

	jOut["Entities"] = nlohmann::json::array();
	auto& entities = EntityManager::getInstance().getExistingEntities();
	for (auto ID : entities)
	{
		Entity ent(ID);

		nlohmann::json jEnt;
		jEnt["Components"] = nlohmann::json::array();

		if (ent.hasComponent<MarbleComponent>() || ent.hasComponent<Emitter>())
			continue;

		if (ent.hasComponent<MarbleShooter>())
			jEnt["Components"].push_back(ent.getComponent<MarbleShooter>().serialize());
		if (ent.hasComponent<Transform>())
			jEnt["Components"].push_back(ent.getComponent<Transform>().serialize());
		if (ent.hasComponent<Sprite>())
			jEnt["Components"].push_back(ent.getComponent<Sprite>().serialize());
		if (ent.hasComponent<Velocity>())
			jEnt["Components"].push_back(ent.getComponent<Velocity>().serialize());
		/*if (ent.hasComponent<Animation>())
			jEnt["Components"].push_back(ent.getComponent<Animation>().serialize());*/
		if (ent.hasComponent<AABB>())
			jEnt["Components"].push_back(ent.getComponent<AABB>().serialize());
		if (ent.hasComponent<Emitter>())
			jEnt["Components"].push_back(ent.getComponent<Emitter>().serialize());
		if (ent.hasComponent<RenderingLayer>())
			jEnt["Components"].push_back(ent.getComponent<RenderingLayer>().serialize());

		jOut["Entities"].push_back(jEnt);
	}

	std::ofstream file(filepath);
	file <<jOut.dump(4);
}

void GeneralZumaScene::deSerialize(std::string filepath)
{
	std::ifstream file(filepath);
	if (!file.is_open())
		return;

	auto& routeSysBin = SystemsManager::getInstance().getSystemBin<RouteS>();
	for (auto& route : routeSysBin)
		SystemsManager::getInstance().deleteSystem<RouteS>(route.first);

	SystemsManager::getInstance().update();
	EntityManager::getInstance().update();

	nlohmann::json j = nlohmann::json::parse(file);

	for (int i = 0; i < j["Routes"].size(); i++)
	{
		RouteS* newRoute = new RouteS(j["Routes"][i]);
		SystemsManager::getInstance().addSystem<RouteS>(newRoute, newRoute->getID());
	}

	EntityManager::getInstance().reset();

	for (int i = 0; i < j["Entities"].size(); i++)
	{
		Entity newEnt = EntityManager::getInstance().createEntity();

		nlohmann::json& jEnt = j["Entities"][i]["Components"];
		for (int i2 = 0; i2 < jEnt.size(); i2++)
		{
			nlohmann::json& jComp = jEnt[i2];
			auto str = jEnt[i2].dump(4);
			int type = jComp["type"];
			switch (type)
			{
			case CBase::MarbleShooter:
			{
				MarbleShooter comp;
				comp.deSerialize(jComp);
				newEnt.addComponent<MarbleShooter>(comp);
			}
			break;
			case CBase::Transform:
			{
				Transform comp;
				comp.deSerialize(jComp);
				newEnt.addComponent<Transform>(comp);
			}
			break;
			case CBase::Sprite:
			{
				Sprite comp;
				comp.deSerialize(jComp);
				newEnt.addComponent<Sprite>(comp);
			}
			break;
			case CBase::Velocity:
			{
				Velocity comp;
				comp.deSerialize(jComp);
				newEnt.addComponent<Velocity>(comp);
			}
			break;
			//case CBase::Animation:
			//{
			//	Animation comp;
			//	comp.deSerialize(jComp);
			//	newEnt.addComponent<Animation>(comp);
			//}
			//break;
			case CBase::AABB:
			{
				AABB comp;
				comp.deSerialize(jComp);
				newEnt.addComponent<AABB>(comp);
			}
			break;
			case CBase::Emitter:
			{
				Emitter comp;
				comp.deSerialize(jComp);
				newEnt.addComponent<Emitter>(comp);
			}
			break;
			case CBase::RenderingLayer:
			{
				RenderingLayer comp;
				comp.deSerialize(jComp);
				newEnt.addComponent<RenderingLayer>(comp);
			}
			break;
			default:
				break;
			}
		}

		if (!newEnt.hasComponent<Velocity>())
			newEnt.addComponent<Velocity>(Velocity());

		SystemsManager::getInstance().getSystem<RenderingS>()->addEntity(newEnt.getID());
		SystemsManager::getInstance().getSystem<ShooterS>()->addEntity(newEnt.getID());
		SystemsManager::getInstance().getSystem<AnimationS>()->addEntity(newEnt.getID());
		SystemsManager::getInstance().getSystem<MovementS>()->addEntity(newEnt.getID());
		SystemsManager::getInstance().getSystem<CollisionS>()->addEntity(newEnt.getID());
		SystemsManager::getInstance().getSystem<ParticleS>()->addEntity(newEnt.getID());
	}
}

void GeneralZumaScene::updateMarbleArchetypes(std::string folderpath)
{
	marbleArchetypeFilepaths.clear();
	for (const auto& entry : std::filesystem::directory_iterator(folderpath))
	{
		marbleArchetypeFilepaths.push_back(entry.path().string());
	}
}
