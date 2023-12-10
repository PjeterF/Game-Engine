#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "src/Graphics/SpriteRenderer.hpp"
#include "src/Graphics/LineRenderer.hpp"
#include "src/Graphics/TextRenderer.hpp"
#include "src/Graphics/QuadRenderer.hpp"
#include "src/Graphics/GeneralRenderer.hpp"
#include "src/OpenGL/ShaderProgram.hpp"
#include "src/Managers/TexturesManager.hpp"
#include "src/OpenGL/FrameBuffer.hpp"

#include "src/GUI/Viewport.hpp"
#include "src/GUI/PropertiesMenu.hpp"
#include "src/GUI/EditMenu.hpp"
#include "src/GUI/Subcomponents/TextureLoader.hpp"
#include "src/GUI/Subcomponents/SegmentEditor.hpp"

#include "src/Graphics/Camera.hpp"

#include "src/Events/EventManager.hpp"

#include <string>
#include <vector>

#include "src/Events/EventPackage.hpp"
#include "src/ZumaLogic/Shooter.hpp"

#include "src/Managers/Collision/CollisionGrid.hpp"

class Application : public EventObserver
{
public:
	Application(float windowWidth, float windowHeight, std::string windowName);
	void run();
	virtual void handleEvent(Event& event) override;
private:
	void initializeImGui();

	GLFWwindow* window;
	float wndWidth;
	float wndHeight;

	RenderingAPI* generalRenderer;
	Camera* camera;

	EditMenu* mainMenu;
	Viewport* viewport;
	PropertiesMenu* propertiesMenu;

	FrameBuffer* viewportFramebuffer;
	
	bool paused = true;
	bool showCollisions = false;
	bool gameOver = false;
};

