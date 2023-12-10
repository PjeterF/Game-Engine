#include "SegmentEditor.hpp"

#include "../../Managers/TexturesManager.hpp"
#include "../../ZumaLogic/Segment.hpp"

SegmentEditor::SegmentEditor(std::string name) : Subcomponent(name)
{

}

void SegmentEditor::render()
{
	ImVec2 wndSize = ImGui::GetWindowSize();

	if (ImGui::Button("Create segment"))
	{
		Segment::segmentTemplates.push_back(SegmentTemplate(ResourceManager::getInstance().getResource<Texture>("default"), 1, 1, 1, 10, -1));
		EventManager::getInstance().notify(Event(Event::Reinitialize, nullptr));
	}

	ImGui::Text("Current segments");

	static SegmentTemplate* segTemplate = nullptr;
	static int segTemplateIndex = 0;
	int i = 0;
	for (auto it = Segment::segmentTemplates.begin(); it != Segment::segmentTemplates.end(); it++)
	{
		if ((*it).texture->getContents() != nullptr)
		{
			if (ImGui::ImageButton((ImTextureID)(*it).texture->getContents()->getId(), ImVec2(50, 50)))
			{
				segTemplate = &*it;
				segTemplateIndex = i;
				ImGui::OpenPopup("Editor");
			}
		}
		else
		{
			if (ImGui::ImageButton((ImTextureID)0, ImVec2(50, 50)))
			{
				segTemplate = &*it;
				segTemplateIndex = i;
				ImGui::OpenPopup("Editor");
			}
		}

		i++;
	}

	if (ImGui::BeginPopupModal("Editor", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		static int tag = -1;
		ImGui::InputInt("Tag", &tag);
		static int size = 30;
		ImGui::InputInt("Size", &size);
		static int frameDuration = 10;
		ImGui::InputInt("Frame duration", &frameDuration);
		static int dimensions[2] = {1, 1};
		ImGui::InputInt2("Sprite sheet dimensions", dimensions);

		static Resource<Texture>* texture=ResourceManager::getInstance().getResource<Texture>("default");
		if(texture->getContents()!=nullptr)
			ImGui::Image((ImTextureID)texture->getContents()->getId(), ImVec2(30, 30));
		else
			ImGui::Image((ImTextureID)0, ImVec2(30, 30));
		ImGui::SameLine();

		if (ImGui::BeginCombo("Texture", nullptr, ImGuiComboFlags_NoPreview))
		{
			auto it = ResourceManager::getInstance().begin<Texture>();
			while (it != ResourceManager::getInstance().end<Texture>())
			{
				std::string name = " * " + (*it).first;
				if (ImGui::Selectable(name.c_str()))
				{
					texture = (Resource<Texture>*)(*it).second;
				}
				ImGui::SameLine();

				Resource<Texture>* res = (Resource<Texture>*)(*it).second;
				if (res->getContents() == nullptr)
					ImGui::Image((ImTextureID)0, ImVec2(10, 10));
				else
					ImGui::Image((ImTextureID)res->getContents()->getId(), ImVec2(10, 10));

				ImGui::SameLine();
				ImGui::Text(" ");
				it++;
			}
			ImGui::EndCombo();
		}

		if (ImGui::Button("Commit changes"))
		{
			Segment::segmentTemplates[segTemplateIndex] = SegmentTemplate(texture, dimensions[0], dimensions[1], frameDuration, size, tag);
			EventManager::getInstance().notify(Event(Event::Reinitialize, nullptr));
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();

		ImGui::Dummy(ImVec2(50, 0));

		ImGui::SameLine();
		if (ImGui::Button("Delete"))
		{
			for (auto it = Segment::segmentTemplates.begin(); it != Segment::segmentTemplates.end(); it++)
			{
				if (&*it == segTemplate)
				{
					Segment::segmentTemplates.erase(it);
					EventManager::getInstance().notify(Event(Event::Reinitialize, nullptr));
					break;
				}
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();

		ImGui::Dummy(ImVec2(50, 0));

		ImGui::SameLine();
		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}
