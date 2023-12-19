#include "AssetLoader.hpp"

AssetLoader::AssetLoader(int x, int y, int width, int height) : InterfaceWindow(x, y, width, height)
{
	name = "Asset Loader";
}

void AssetLoader::draw()
{
	ImVec2 windowSize = ImGui::GetWindowSize();

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(width, height));
	ImGui::SetNextWindowPos(ImVec2(x, y));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	static char filepath[100] = "";
	ImGui::PushItemWidth(windowSize.x / 2);
	ImGui::InputText("Filepath", filepath, 100);
	ImGui::PopItemWidth();

	if (ImGui::Button("Load Texture", ImVec2(windowSize.x / 2, 20)))
	{
		ResourceManager::getInstance().createResourceFromFile<Texture>(filepath);
		std::fill_n(filepath, 100, 0);
	}
	ImGui::Separator();

	ImGui::BeginChild("Texture list");

	std::string n = "";
	for (auto it = ResourceManager::getInstance().begin<Texture>(); it != ResourceManager::getInstance().end<Texture>(); it++)
	{
		Resource<Texture>* tex = (Resource<Texture>*)(*it).second;
		if (tex->getContents() != nullptr && (*it).first!="defualt")
		{
			ImGui::Image((ImTextureID)tex->getContents()->getId(), ImVec2(100, 100));
			ImGui::SameLine();
			if(ImGui::Button(("Delete" + n).c_str(), ImVec2(100, 30)))
			{
				ResourceManager::getInstance().deleteResource<Texture>(tex->getContents()->getFilepath());
			}
		}
		n.append(" ");
	}
	ImGui::EndChild();

	ImGui::End();
	ImGui::PopStyleVar(1);
}
