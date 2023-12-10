#include "TextureLoader.hpp"

TextureLoader::TextureLoader(std::string name, ResourceManager* texturesManager, int imageSize) : Subcomponent(name)
{
	this->texturesManager = texturesManager;
	this->imageSize = imageSize;
}

void TextureLoader::render()
{	
	int windowWidth = ImGui::GetWindowSize().x;
	maxImagesPerRow = windowWidth / imageSize;

	if (maxImagesPerRow <= 0)
	{
		maxImagesPerRow = 1;
	}

	if (ImGui::Button("Load Texture", ImVec2(windowWidth- 150, 20)))
	{
		texturesManager->createResourceFromFile<Texture>(filepath);
		std::fill_n(filepath, 100, 0);
	}
	ImGui::PushItemWidth(windowWidth-150);
	ImGui::InputText("Filepath", filepath, 100);
	ImGui::PopItemWidth();

	auto it = ResourceManager::getInstance().begin<Texture>();
	while (it != ResourceManager::getInstance().end<Texture>())
	{
		Resource<Texture>* textureRes = (Resource<Texture>*)(*it).second;
		if (textureRes->getContents() != nullptr)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (ImGui::ImageButton((ImTextureID)textureRes->getContents()->getId(), ImVec2(imageSize, imageSize), ImVec2(0, 0), ImVec2(1, -1)))
			{
				ResourceManager::getInstance().deleteResource<Texture>((*it).first);
			}
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::Text((*it).first.c_str());
		}
		it++;
	}
}