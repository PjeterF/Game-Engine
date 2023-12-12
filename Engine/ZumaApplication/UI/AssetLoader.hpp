#pragma once

#include "InterfaceWindow.hpp"
#include "../../src/Managers/Resource/ResourceManager.hpp"

class AssetLoader : public InterfaceWindow
{
public:
	AssetLoader(int x, int y, int width, int height);
	virtual void draw() override;
};