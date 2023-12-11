#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <string>

class InterfaceWindow
{
public:
	InterfaceWindow(int x, int y, int width, int height);
	virtual void draw() = 0;
	void setName(std::string newName);

	int x, y, width, height;
	std::string name;
protected:
	static int nextID;
};