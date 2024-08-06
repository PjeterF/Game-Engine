#include "TextureManager.hpp"
#include "../../src/Managers/Resource/ResourceManager.hpp"
#include <fstream>
#include <algorithm>

TextureManager::TextureManager(std::string name, int x, int y, int width, int height) : UIWindow(name, x, y, width, height)
{
	std::fill(this->name, this->name + buffSize - 1, '\0');
}

void TextureManager::render()
{
	windowBegin();

	ImGui::InputText("Texture file path", &name[0], buffSize-1);
	if (ImGui::Button("Load"))
	{
		std::ifstream file(name);
		if (file.is_open())
		{
			ResourceManager::getInstance().createResourceFromFile<Texture>(name);
			std::fill(name, name + buffSize - 1, '\0');
		}
		file.close();
	}

	auto it = ResourceManager::getInstance().begin<Texture>();
	while (it != ResourceManager::getInstance().end<Texture>())
	{
		Resource<Texture>* textureRes = (Resource<Texture>*)(*it).second;
		Texture* texture = textureRes->getContents();

		if (texture == nullptr) {
			it++;
			continue;
		}

		ImGui::ImageButton((ImTextureID)texture->getId(), ImVec2(50, 50), ImVec2(0, 0), ImVec2(-1, -1));

		it++;
	}
	
	windowEnd();
}
