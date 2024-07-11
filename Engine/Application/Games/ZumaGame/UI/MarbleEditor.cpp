#include "MarbleEditor.hpp"

#include "../../src/Managers/Resource/ResourceManager.hpp"
#include "../../src/Events/EventManager.hpp"

#include <cstdio>
#include <cstring>

namespace fs = std::filesystem;

MarbleEditor::MarbleEditor(std::string name, int x, int y, int width, int height, std::string archetypeDirectoryPath)
	:UIWindow(name, x, y, width, height), archetypeDirectoryPath(archetypeDirectoryPath)
{
	for (int i = 0; i < 100; i++)
		newName[i]=' ';
}

void MarbleEditor::render()
{
	windowConfig();

	fs::path dirPath(archetypeDirectoryPath);

	if (ImGui::Button("Create new archetype"))
	{
		std::ofstream outFile(archetypeDirectoryPath + "/newArchetype.json");

		MarbleArchetype newArchetype;

		outFile << newArchetype.serialize().dump(4);
		outFile.close();
	}

	for (auto& entry : fs::directory_iterator(dirPath))
	{
		std::string fileName = entry.path().string();
		fileName.erase(0, archetypeDirectoryPath.size() + 1);
		fileName.pop_back();
		fileName.pop_back();
		fileName.pop_back();
		fileName.pop_back();
		fileName.pop_back();
		if (ImGui::Selectable(fileName.c_str()))
		{
			strcpy(newName, fileName.c_str());
			selectedArchetypePath = entry.path().string();
			std::replace(selectedArchetypePath.begin(), selectedArchetypePath.end(), '\\', '/');
			file.open(selectedArchetypePath);
			j = nlohmann::json::parse(file);

			std::vector<glm::vec4> frames;
			for (int i = 0; i < j["frames"].size(); i++)
				frames.push_back({ j["frames"][i][0], j["frames"][i][1], j["frames"][i][2], j["frames"][i][3] });

			marbleArchetype = MarbleArchetype(
				{ j["color"][0], j["color"][1], j["color"][2] },
				j["frameDuration"],
				j["size"],
				j["type"],
				j["texture"],
				frames
			);

			if (file.is_open()) 
			{
				file.close();
				ImGui::OpenPopup("Marble Attributes");
			}
				
		}
	}

	if (ImGui::BeginPopupModal("Marble Attributes"))
	{
		hovered = ImGui::IsWindowHovered();


		ImGui::Text("File name");
		ImGui::InputText("##File Name", &newName[0], 100);

		ImGui::Separator();
		ImGui::Text("Attributes");
		ImGui::Separator();
		ImGui::InputFloat3("Color", &marbleArchetype.color[0]);
		ImGui::InputFloat("Size", &marbleArchetype.size);
		ImGui::InputFloat("Frame duration", &marbleArchetype.frameDuration);
		ImGui::InputInt("Type", &marbleArchetype.type);

		ImGui::Separator();
		ImGui::Text("Texture");
		ImGui::Separator();
		ImGui::Image((ImTextureID)ResourceManager::getInstance().getResource<Texture>(marbleArchetype.texture)->getContents()->getId(), ImVec2(30, 30));
		if (ImGui::BeginCombo("Choose texture", nullptr, ImGuiComboFlags_NoPreview))
		{
			auto it = ResourceManager::getInstance().begin<Texture>();
			auto it_end = ResourceManager::getInstance().end<Texture>();
			while (it != it_end)
			{
				if ((*it).first == "" || (*it).first == "default") {
					it++;
					continue;
				}
					
				Resource<Texture>* texture = (Resource<Texture>*)(*it).second;
				if (ImGui::Selectable((*it).first.c_str()))
				{
					marbleArchetype.texture = (*it).first;
				}
				it++;
			}
			ImGui::EndCombo();
		}

		ImGui::Separator();
		ImGui::Text("Frames");
		ImGui::SameLine();
		if (ImGui::Button("Add"))
			marbleArchetype.frames.push_back({ 0, 0, 0, 0 });
		ImGui::Separator();

		ImGui::Columns(5, "divisions");
		ImGui::Text("x");
		for (int i = 0; i < marbleArchetype.frames.size(); i++)
		{
			std::string label = "##x:" + std::to_string(i);
			ImGui::InputFloat(label.c_str(), &marbleArchetype.frames[i].x);
		}
		ImGui::NextColumn();
		ImGui::Text("y");
		for (int i = 0; i < marbleArchetype.frames.size(); i++)
		{
			std::string label = "##y:" + std::to_string(i);
			ImGui::InputFloat(label.c_str(), &marbleArchetype.frames[i].y);
		}
		ImGui::NextColumn();
		ImGui::Text("width");
		for (int i = 0; i < marbleArchetype.frames.size(); i++)
		{
			std::string label = "##width:" + std::to_string(i);
			ImGui::InputFloat(label.c_str(), &marbleArchetype.frames[i].z);
		}
		ImGui::NextColumn();
		ImGui::Text("height");
		for (int i = 0; i < marbleArchetype.frames.size(); i++)
		{
			std::string label = "##height:" + std::to_string(i);
			ImGui::InputFloat(label.c_str(), &marbleArchetype.frames[i].w);
		}
		ImGui::NextColumn();
		ImGui::Text("action");
		for (int i = 0; i < marbleArchetype.frames.size(); i++)
		{
			std::string label = "Delete##" + std::to_string(i);
			if (ImGui::Button(label.c_str()))
			{
				marbleArchetype.frames.erase(std::next(marbleArchetype.frames.begin(), i));
			}
		}
		ImGui::NextColumn();
		ImGui::Columns(1);

		ImGui::Separator();
		if (ImGui::Button("Close", ImVec2(0, 20)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Commit changes", ImVec2(0, 20)))
		{
			fs::remove(fs::path(selectedArchetypePath));

			std::string outputPath = archetypeDirectoryPath + "/" + newName + ".json";
			std::ofstream outputFile;
			outputFile.open(outputPath);
			outputFile << marbleArchetype.serialize().dump(4);
			if (outputFile.is_open())
			{
				
			}
			outputFile.close();

			EventManager::getInstance().notify(Event(Event::UpdateMarbleArchetypes, nullptr), ECS2);

			ImGui::CloseCurrentPopup();
		}
		ImGui::Separator();
		ImGui::EndPopup();
	}

	windowEnd();
}

nlohmann::json MarbleEditor::MarbleArchetype::serialize()
{
	nlohmann::json j;

	j["color"][0] = color[0];
	j["color"][1] = color[1];
	j["color"][2] = color[2];

	j["size"] = size;
	j["frameDuration"] = frameDuration;
	j["texture"] = texture;
	j["type"] = type;

	j["frames"] = nlohmann::json::array();
	for (auto& frame : frames)
		j["frames"].push_back({ frame.x, frame.y, frame.z, frame.w });

	return j;
}
