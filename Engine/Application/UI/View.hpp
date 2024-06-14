#pragma once

#include "UIElement.hpp"

class View : public UIElement
{
public:
	View(glm::vec2 position, glm::vec2 dimensions);
	void run() override;
	unsigned int textureID;
};