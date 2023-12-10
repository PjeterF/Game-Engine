#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "../../ZumaLogic/CubicBezierSpline.hpp"
#include "../Viewport.hpp"
#include "Subcomponent.hpp"

class RouteEdit : public Subcomponent
{
public:
	RouteEdit(std::string name, CubicBezierSpline* spline, Viewport* viewport);
	virtual void render() override;
private:
	CubicBezierSpline* route;
	Viewport* viewport;
};

