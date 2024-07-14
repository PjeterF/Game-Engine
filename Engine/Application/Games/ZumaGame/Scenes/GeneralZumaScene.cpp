#include "GeneralZumaScene.hpp"

#include "../../src/Input/Windows/GLFWInputManager.hpp"

#include "../ECS/Systems/RouteS.hpp"
#include "../ECS/Systems/MarbleCollisionResolutionS.hpp"

GeneralZumaScene::GeneralZumaScene(Camera& camera) : Scene(camera)
{
}

void GeneralZumaScene::initialize()
{
	glm::ivec2 windowDimensions = GLFWInputManager::getInstance().getWindowDimensions();

	UIElements.push_back(new MarbleEditor("Marble Editor", 0, 0, 300, windowDimensions.y/2, "Application/Games/ZumaGame/MarbleArchetypes"));
	UIElements.push_back(new ZumaMenu("Zuma Menu", 0, windowDimensions.y / 2, 300, windowDimensions.y / 2, systems[std::type_index(typeid(RouteS))]));

	std::vector<glm::vec2> ctrlPts = {
		{100, 100}, 
		{200, 400},
		{300, 500}, 
		{700, 400},
		{1100, 300},
		{1200, 500},
		{1300, 100}
	};
	addSystem<RouteS>("Route1", new RouteS("Route1", ctrlPts, 10, 2, 10), true);
	addSystem<CollisionS>("Col", new CollisionS(30), true);
	addSystem<MovementS>("Mov", new MovementS(), true);
	addSystem<AnimationS>("Anim", new AnimationS(), true);
	addSystem<MarbleCollisionResolutionS>("ColRes", new MarbleCollisionResolutionS(systems[std::type_index(typeid(RouteS))]));

	std::vector<std::string> marbleArchetypeFilepaths = {
		"Application/Games/ZumaGame/MarbleArchetypes/marble1.json",
		"Application/Games/ZumaGame/MarbleArchetypes/marble2.json",
		"Application/Games/ZumaGame/MarbleArchetypes/marble3.json",
	};
	addSystem<ShooterS>("S0", new ShooterS(
		marbleArchetypeFilepaths,
		*getSystem<MovementS>("Mov"),
		*getSystem<RenderingS>("Rend"),
		*getSystem<CollisionS>("Col"),
		*getSystem<AnimationS>("Anim"), 
		*getSystem<MarbleCollisionResolutionS>("ColRes")), true);

	Entity shooter = EntityManager::getInstance().createEntity();
	shooter.addComponent<Transform>(Transform(1000, 1000, 100, 100));
	shooter.addComponent<Velocity>(Velocity());
	shooter.addComponent<RenderingLayer>(RenderingLayer());
	shooter.addComponent<MarbleShooter>(MarbleShooter(6, 30));
	shooter.addComponent<Sprite>(Sprite(ResourceManager::getInstance().getResource<Texture>("Application/Games/ZumaGame/Textures/frog.png")));

	getSystem<RenderingS>("Rend")->addEntity(shooter.getID());
	getSystem<ShooterS>("S0")->addEntity(shooter.getID());

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
}

void GeneralZumaScene::update(float dt)
{
	static std::vector<std::string> marbleArchetypes = {
		"Application/Games/ZumaGame/MarbleArchetypes/marble1.json",
		"Application/Games/ZumaGame/MarbleArchetypes/marble2.json",
		"Application/Games/ZumaGame/MarbleArchetypes/marble3.json"
	};

	for (auto& item : systems[std::type_index(typeid(RouteS))])
	{
		((RouteS*)item.second)->spawnMarbleIfPossible(
			marbleArchetypes[rand() % marbleArchetypes.size()],
			*getSystem<MovementS>("Mov"),
			*getSystem<RenderingS>("Rend"),
			*getSystem<CollisionS>("Col"),
			*getSystem<AnimationS>("Anim")
		);
	}

	getSystem<CollisionS>("Col")->update(dt);

	getSystem<ShooterS>("S0")->update(dt);
	for (auto& item : systems[std::type_index(typeid(RouteS))])
	{
		RouteS* route = (RouteS*)item.second;
		route->update(dt);
	}
	getSystem<MarbleCollisionResolutionS>("ColRes")->update(dt);
	getSystem<AnimationS>("Anim")->update(dt);
	getSystem<MovementS>("Mov")->update(dt);

	getSystem<CollisionS>("Col")->lateUpdate(dt);

	EntityManager::getInstance().update();

	for(int i=0;i<10;i++)
		emitter->emit();
	emitter->applyForceInverseToSize(0.1, 0.1);
	emitter->update();
}

void GeneralZumaScene::draw(RenderingAPI* renderingAPI)
{
	for (auto& item : systems[std::type_index(typeid(RouteS))])
	{
		((RouteS*)item.second)->draw(renderingAPI);
	}

	getSystem<RenderingS>("Rend")->update(0);

	for (auto& element : UIElements)
		element->render();

	emitter->draw(renderingAPI);
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
		auto it = systems[std::type_index(typeid(RouteS))].find(selectedRoute);
		if(it != systems[std::type_index(typeid(RouteS))].end())
			getSystem<RouteS>(selectedRoute)->addSegment(cursorPos);
	}
		
	if (input.keyClicked[ZE_KEY_E])
	{
		auto it = systems[std::type_index(typeid(RouteS))].find(selectedRoute);
		if (it != systems[std::type_index(typeid(RouteS))].end())
			getSystem<RouteS>(selectedRoute)->removeLastSegment();
	}
	if (input.mouseKeyClicked[ZE_MOUSE_BUTTON_1])
		EventManager::getInstance().notify(Event(Event::Shoot, &cursorPos), ECS2);

	if (movingPt)
	{
		auto route = getSystem<RouteS>(selectedRoute);
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
			for (auto& route : systems[std::type_index(typeid(RouteS))])
			{
				int result = ((RouteS*)route.second)->ctrlPointIntersection(cursorPos);
				if (result == -1)
					continue;

				selectedRoute = route.first;
				ctrlPtIdx = result;
				movingPt = true;
				break;
			}
		}
	}

	input.update();
}