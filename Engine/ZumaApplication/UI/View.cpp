#include "View.hpp"
#include <iostream>
View::View(int x, int y, int width, int height, unsigned int textureID, GLFWwindow* window, Camera* camera, InputManager* input) : InterfaceWindow(x, y, width, height)
{
	this->textureID = textureID;
	this->window = window;
	this->camera = camera;
	this->input = input;
}

void View::draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(width, height));
	ImGui::SetNextWindowPos(ImVec2(x, y));
	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	ImGui::Image((ImTextureID)textureID, ImVec2(width, height), ImVec2(0, 0), ImVec2(1, -1));

	if (ImGui::IsItemHovered())
	{
		float w = input->mouseWheel();
		if (w)
		{
			camera->changeZoom(0.05f * w);
		}
		if (input->wasMouseButtonPressed(ZE_MOUSE_BUTTON_MIDDLE))
		{
			panning = true;
			panAnchor = input->getCursorPos();
		}
		if(!input->isMouseButtonPressed(ZE_MOUSE_BUTTON_MIDDLE))
		{
			panning = false;
		}
		if (panning)
		{
			glm::vec2 pan = (input->getCursorPos() - panAnchor) / camera->getZoom();
			camera->pan(pan.x, pan.y);
		}
		if (input->wasMouseButtonPressed(ZE_MOUSE_BUTTON_1))
		{
			glm::vec2 mousePos = input->getCursorPos();
			glm::vec2 conv = convertPos(io.MousePos.x, io.MousePos.y);
			EventManager::getInstance().notify(Event(Event::MouseClick, &conv), UI);
			EventManager::getInstance().notify(Event(Event::MouseClick, &conv), ECS);
		}
		if (mouseMoved())
		{
			glm::vec2 mousePos = input->getCursorPos();
			glm::vec2 conv = convertPos(io.MousePos.x, io.MousePos.y);
			EventManager::getInstance().notify(Event(Event::MouseMove, &conv), UI);
		}
	}

	ImGui::End();
	ImGui::PopStyleVar(2);
}

glm::vec2 View::convertPos(float x, float y)
{
	x = x - camera->getOffset().x;
	y = y + camera->getOffset().y;

	float xScale = (x - this->x) / width;
	float yScale = (y - this->y) / height;

	x = width * xScale / camera->getZoom();
	y = height * yScale / camera->getZoom();

	y = height / camera->getZoom() - y;

	return glm::vec2(x, y);
}

bool View::mouseMoved()
{
	ImGuiIO& io = ImGui::GetIO();

	if (prevMousePos.x != io.MousePos.x || prevMousePos.y != io.MousePos.y)
	{
		prevMousePos.x = io.MousePos.x;
		prevMousePos.y = io.MousePos.y;
		return true;
	}
	else
	{
		return false;
	}
}
