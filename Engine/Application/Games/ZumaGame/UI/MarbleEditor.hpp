#pragma once

#include "UIWindow.hpp"

#include <filesystem>

class MarbleEditor : public UIWindow
{
public:
	MarbleEditor(std::string name, int x, int y, int width, int height, std::string archetypeDirectoryPath);
	virtual void render() override;
private:
	std::string archetypeDirectoryPath;
};