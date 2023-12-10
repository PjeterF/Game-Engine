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
		if (input->wasMouseButtonPressed(ZE_MOUSE_BUTTON_1))
		{
			std::cout << "WORKS";
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
