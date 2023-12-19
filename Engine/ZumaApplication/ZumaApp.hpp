#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../src/Graphics/GeneralRenderer.hpp"
#include "../src/ECS/Systems/CollisionSystem.hpp"
#include "../src/ECS/Systems/MovementSystem.hpp"
#include "../src/ECS/Systems/SpriteRenderingSystem.hpp"
#include "../src/ECS/Systems/TestCollisionResponse.hpp"
#include "../src/ECS/Systems/InputMovementSystem.hpp"
#include "../src/ECS/Systems/LayeredRenderingSystem.hpp"
#include "../src/ECS/Systems/ParticleSystem.hpp"
#include "../src/ECS/Systems/CounterKillerSystem.hpp"

#include "../src/OpenGL/FrameBuffer.hpp"

#include "../src/Input/Windows/GLFWInputManager.hpp"

#include "Logic/RouteManagementSystem.hpp"
#include "Logic/ShooterManagementSystem.hpp"
#include "Logic/MarbleCollisionResolutionSystem.hpp"
#include "Logic/Spline.hpp"
#include "Logic/ShooterInfoC.hpp"

#include "UI/EntityPropertiesMenu.hpp"
#include "UI/SceneMenu.hpp"
#include "UI/View.hpp"
#include "UI/AssetLoader.hpp"
#include "UI/ZumaMenu.hpp"
#include "UI/TopBar.hpp"

#include <string>

class ZumaApp : public EventObserver
{
public:
	ZumaApp(float windowWidth, float windowHeight, std::string windowName);
	void run();

	virtual void handleEvent(Event& event) override;

private:
	void initializeImGui();
	void updateSystems(float dt);
	Ent* getEntity(int id);
	void removeEntity(int id);

	std::vector<SystemBase*> systems;
	std::unordered_map<int, Ent*> entities;

	GLFWwindow* window;
	float wndWidth;
	float wndHeight;

	EntityPropertiesMenu* propertiesMenu;
	SceneMenu* sceneMenu;
	View* view;
	AssetLoader* assetLoader;
	ZumaMenu* zumaMenu;
	TopBar* bar;

	FrameBuffer* viewportFramebuffer;

	RenderingAPI* renderingAPI=nullptr;
	Camera* mainCamera=nullptr;

	InputManager* inputManager;

	std::vector<RouteManagementSystem*> routes;

	bool paused = true;
};