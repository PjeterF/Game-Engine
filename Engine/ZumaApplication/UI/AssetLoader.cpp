#include "AssetLoader.hpp"

AssetLoader::AssetLoader(int x, int y, int width, int height) : InterfaceWindow(x, y, width, height)
{
	name = "Asset Loader";
}

void AssetLoader::draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(width, height));
	ImGui::SetNextWindowPos(ImVec2(x, y));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	ImGui::Text("TEST");
	ImGui::Separator();

	ImGui::BeginChild("Texture list");
	for (auto it = ResourceManager::getInstance().begin<Texture>(); it != ResourceManager::getInstance().end<Texture>(); it++)
	{
		Resource<Texture>* tex = (Resource<Texture>*)(*it).second;

		if (tex->getContents() != nullptr)
			ImGui::Image((ImTextureID)tex->getContents()->getId(), ImVec2(100, 100));
	}
	ImGui::EndChild();

	ImGui::End();
	ImGui::PopStyleVar(1);
}
