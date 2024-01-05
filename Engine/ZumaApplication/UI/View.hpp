#pragma once

#include "InterfaceWindow.hpp"
#include "../../src/Graphics/Camera.hpp"
#include "../../src/Input/InputManager.hpp"

class View : public InterfaceWindow, public EventObserver
{
public:
	View(int x, int y, int width, int height, unsigned int textureID, GLFWwindow* window, Camera* camera, InputManager* input);
	virtual void draw() override;

	virtual void handleEvent(Event& event) override;
private:
	enum state { def, drag };

	glm::vec2 convertPos(float x, float y);

	glm::vec2 windowToViewToWorldCoordConversion(float x, float y);

	ImGuiIO& io = ImGui::GetIO();

	bool mouseMoved();
	glm::vec2 prevMousePos = glm::vec2(0, 0);

	unsigned int textureID;
	GLFWwindow* window;
	Camera* camera;
	InputManager* input;

	bool panning = false;
	bool dragging = false;
	glm::vec2 panAnchor = glm::vec2(0, 0);

	state st = drag;

	bool paused = true;
};