#pragma once
#include <string>

#include "Subcomponent.hpp"
#include "../../ZumaLogic/Segment.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class SegmentEditor : public Subcomponent
{
public:
	SegmentEditor(std::string name);
	virtual void render() override;
	int imageSize = 30;
private:

};