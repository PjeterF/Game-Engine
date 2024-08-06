#pragma once

#include "UIWindow.hpp"

class TextureManager : public UIWindow
{
public:
	TextureManager(std::string name, int x, int y, int width, int height);
	virtual void render() override;
private:
	static const int buffSize = 300;
	char name[buffSize];
};