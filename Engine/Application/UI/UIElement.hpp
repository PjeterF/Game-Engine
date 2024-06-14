#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glm/glm.hpp>

class UIElement
{
public:
	UIElement(glm::vec2 position, glm::vec2 dimensions) : position(position), dimensions(dimensions) {}
	virtual void run() = 0;
	glm::vec2 position;
	glm::vec2 dimensions;
};