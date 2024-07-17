#pragma once

#include <string>
#include <glm/glm.hpp>

struct MarbleComponent
{
	MarbleComponent(int type = -1, glm::vec3 color = {1, 1, 1}, std::string routeName = "", int targetPointIdx = -1, bool separated = false)
		: type(type), routeName(routeName), targetPointIdx(targetPointIdx), separated(separated), color(color) {}
	int type;
	glm::vec3 color;
	std::string routeName;
	int targetPointIdx;
	bool separated;
};