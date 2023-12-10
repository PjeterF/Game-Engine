#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "../OpenGL/FrameBuffer.hpp"
#include "../Events/EventPackage.hpp"

#include "GUI_Window.hpp"
#include "../Graphics/Camera.hpp"
#include "../Managers/EntityManager.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Viewport : public GUI_Window, public EventObserver
{
public:
	enum InputStates
	{
		Nothing, AddSegment, Shoot, MoveCtlrPoints
	};

	Viewport(int xmin, int ymin, int xman, int ymax, unsigned int textureID, GLFWwindow* window, Camera* camera);
	void render();
	void setTexture(unsigned int textureID);
	InputStates getState() { return inputState; }
	void setState(InputStates state) { inputState = state; }
	virtual void handleEvent(Event& event) override;
private:
	glm::vec2 convertPos(float x, float y);
	Entity* selectEntity(float x, float y);

	ImGuiIO& io = ImGui::GetIO();

	bool mouseMoved();
	glm::vec2 prevMousePos = glm::vec2(0, 0);

	unsigned int textureID;
	GLFWwindow* window;
	Camera* camera;
	Entity* selectedEntity = nullptr;

	bool panning = false;
	glm::vec2 panAnchor = glm::vec2(0, 0);

	bool movingCtrlPoint = false;
	bool isHovered = false;
	InputStates inputState = MoveCtlrPoints;

	bool paused = false;
};