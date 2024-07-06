#include "GeneralZumaScene.hpp"

#include "../../src/Input/Windows/GLFWInputManager.hpp"

#include "../ECS/Systems/RouteS.hpp"
#include "../ECS/Systems/MarbleCollisionResolutionS.hpp"

GeneralZumaScene::GeneralZumaScene(Camera& camera) : Scene(camera)
{
}

void GeneralZumaScene::initialize()
{
	UIElements.push_back(new MarbleEditor("Marble Editor", 0, 0, 100, 100));

	std::vector<glm::vec2> ctrlPts = {
		{100, 100}, 
		{200, 400},
		{300, 500}, 
		{1000, 100},
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

	for (int i = 0; i < 1; i++)
	{
		getSystem<RouteS>("Route1")->spawnMarbleAtOrigin(
			"Application/Games/ZumaGame/MarbleArchetypes/marble1.json",
			*getSystem<MovementS>("Mov"),
			*getSystem<RenderingS>("Rend"),
			*getSystem<CollisionS>("Col"),
			*getSystem<AnimationS>("Anim")

		);
	}
}

void GeneralZumaScene::update(float dt)
{
	static int iteration = 9990;
	static std::vector<std::string> marbleArchetypes = {
		"Application/Games/ZumaGame/MarbleArchetypes/marble1.json",
		"Application/Games/ZumaGame/MarbleArchetypes/marble2.json",
		"Application/Games/ZumaGame/MarbleArchetypes/marble3.json"
	};

	getSystem<RouteS>("Route1")->spawnMarbleIfPossible(
		marbleArchetypes[rand()%marbleArchetypes.size()],
		*getSystem<MovementS>("Mov"),
		*getSystem<RenderingS>("Rend"),
		*getSystem<CollisionS>("Col"),
		*getSystem<AnimationS>("Anim")
	);

	getSystem<CollisionS>("Col")->update(dt);

	getSystem<ShooterS>("S0")->update(dt);
	getSystem<RouteS>("Route1")->update(dt);
	getSystem<MarbleCollisionResolutionS>("ColRes")->update(dt);
	getSystem<AnimationS>("Anim")->update(dt);

	getSystem<MovementS>("Mov")->update(dt);

	getSystem<CollisionS>("Col")->lateUpdate(dt);

	EntityManager::getInstance().update();
}

void GeneralZumaScene::draw(RenderingAPI* renderingAPI)
{
	renderingAPI->addQuadInstance({ 100, 100 }, { 50, 50 }, 1, { 1, 1, 1, 1 });
	renderingAPI->drawQuadInstances();

	getSystem<RouteS>("Route1")->draw(renderingAPI);
	getSystem<RenderingS>("Rend")->update(0);

	ImGui::Begin("Debug");
	static int nSamples = 10;
	if (ImGui::InputInt("nSegments", &nSamples))
	{
		getSystem<RouteS>("Route1")->setNSamples(nSamples);
	}
	ImGui::End();

	for (auto& element : UIElements)
		element->render();
}

void GeneralZumaScene::input()
{
	auto& input = GLFWInputManager::getInstance();

	auto cursorPos = (input.getCursorPos() / camera.getZoom() + camera.getPosition());



	if (input.keyClicked[ZE_KEY_SPACE])
	{
		EventManager::getInstance().notify(Event(Event::TogglePause, nullptr), ApplicationBin);
	}
	if (input.keyClicked[ZE_KEY_Q])
	{
		getSystem<RouteS>("Route1")->addSegment(cursorPos);
	}
	if (input.keyClicked[ZE_KEY_E])
	{
		getSystem<RouteS>("Route1")->removeLastSegment();
	}
	if (input.mouseKeyClicked[ZE_MOUSE_BUTTON_1])
	{
		EventManager::getInstance().notify(Event(Event::Shoot, &cursorPos), ECS2);
	}
	if (movingPt)
	{
		auto route = getSystem<RouteS>(selectedRoute);
		route->setControlPointPos(ctrlPtIdx, cursorPos);

		if (input.mouseKeyClicked[ZE_MOUSE_BUTTON_2])
		{
			selectedRoute = "";
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
