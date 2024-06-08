#include "View.hpp"
#include <iostream>
View::View(int x, int y, int width, int height, unsigned int textureID, GLFWwindow* window, Camera* camera, InputManager* input) : InterfaceWindow(x, y, width, height), EventObserver(Defualt)
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

	if (ImGui::BeginPopup("RouteMenu"))
	{
		if (ImGui::Selectable("Add route point"))
		{
			glm::vec2 conv = convertPos(io.MousePos.x, io.MousePos.y);
			EventManager::getInstance().notify(Event(Event::PlacePoint, &conv), UI);
		}
		if (ImGui::Selectable("Remove last route segment"))
		{
			EventManager::getInstance().notify(Event(Event::RemoveLastPoint, nullptr), UI);
		}

		ImGui::EndPopup();
	}

	if (ImGui::IsItemHovered())
	{
		float w = input->mouseWheel();
		if (w)
		{
			camera->changeZoom(0.05f * w);
		}
		if (input->mouseKeyClicked[ZE_MOUSE_BUTTON_MIDDLE])
		{
			panning = true;
			panAnchor = input->getCursorPos();
		}
		if(!input->mouseKeyDown[ZE_MOUSE_BUTTON_MIDDLE])
		{
			panning = false;
		}
		if (input->mouseKeyClicked[ZE_MOUSE_BUTTON_1] && st == drag)
		{
			dragging = true;
			panAnchor = input->getCursorPos();
		}
		if (!input->mouseKeyDown[ZE_MOUSE_BUTTON_1])
		{
			dragging = false;
		}
		if (panning || dragging)
		{
			glm::vec2 pan = 0.2f * (input->getCursorPos() - panAnchor) / camera->getZoom();
			camera->pan(pan.x, pan.y);
		}
		bool btn1Press = input->mouseKeyClicked[ZE_MOUSE_BUTTON_1];
		if (btn1Press && paused == false)
		{
			glm::vec2 conv = convertPos(io.MousePos.x, io.MousePos.y);
			EventManager::getInstance().notify(Event(Event::Shoot, &conv), ECS);
		}
		else if (btn1Press && paused == true)
		{
			glm::vec2 mousePos = input->getCursorPos();
			glm::vec2 conv = convertPos(io.MousePos.x, io.MousePos.y);
			EventManager::getInstance().notify(Event(Event::MouseClick, &conv), UI);
			EventManager::getInstance().notify(Event(Event::MouseClick, &conv), ECS);
		}
		bool btn2Press = input->mouseKeyClicked[ZE_MOUSE_BUTTON_2];
		if(btn2Press)
		{
			ImGui::OpenPopup("RouteMenu");
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

void View::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::Pause:
	{
		paused = true;
	}
	break;
	case Event::Unpause:
	{
		paused = false;
	}
	break;
	}
}

glm::vec2 View::convertPos(float x, float y)
{
	x = x - camera->getPosition().x;
	y = y + camera->getPosition().y;

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
