#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <string>

class UIWindow
{
public:
	UIWindow(std::string name, int x, int y, int width, int height) : x(x), y(y), width(width), height(height), name(name) {}
	virtual void render() = 0;
protected:
	virtual void prelude();
	virtual void epilogue();

	int x, y, width, height;
	std::string name;
};