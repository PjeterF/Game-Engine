#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "../../Managers/TexturesManager.hpp"
#include "Subcomponent.hpp"
#include "../../Managers/Resource/ResourceManager.hpp"


class TextureLoader : public Subcomponent
{
public:
	TextureLoader(std::string name, ResourceManager* texturesManager, int imageSize);
	virtual void render() override;
private:
	ResourceManager* texturesManager;

	char filepath[100] = "";
	char textureName[100] = "";

	int maxImagesPerRow;
	int imageSize;
};

