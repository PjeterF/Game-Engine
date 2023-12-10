#include "SpriteSheetEditor.hpp"
#include "../OpenGL/ShaderProgram.hpp"

SpriteSheetEditor::SpriteSheetEditor(int viewportWidth, int viewPortHeight)
{
	this->viewportWidth = viewportWidth+50;
	this->viewPortHeight = viewPortHeight;

	frameBuffer = new FrameBuffer(viewportWidth, viewPortHeight);
	camera = new Camera(0, viewportWidth, 0, viewPortHeight);
	renderer = new SpriteRenderer(ResourceManager::getInstance().getResource<ShaderProgram>("sprite")->getContents()->getId(), camera);
}

SpriteSheetEditor::~SpriteSheetEditor()
{
	delete camera;
	delete renderer;
	delete frameBuffer;
}

void SpriteSheetEditor::render()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::BeginChild("Editor");

	if (currTexture != nullptr)
	{
		frameBuffer->bind();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2, 0.2, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer->draw
		(
			glm::vec2(50, 100),
			glm::vec2(100, 100),
			0,
			currTexture->getContents()
		);

		frameBuffer->unbind();

		ImGui::Image((ImTextureID)frameBuffer->getTextureID(), ImVec2(viewportWidth, viewPortHeight), ImVec2(0, 0), ImVec2(1, -1));
	}

	ImGui::Separator();

	if (ImGui::BeginCombo("Texture", nullptr, ImGuiComboFlags_NoPreview))
	{
		auto it = ResourceManager::getInstance().begin<Texture>();
		while (it != ResourceManager::getInstance().end<Texture>())
		{
			std::string name = " * " + (*it).first;
			if (ImGui::Selectable(name.c_str()))
			{
				currTexture = (Resource<Texture>*)(*it).second;
				
			}
			ImGui::SameLine();
			Resource<Texture>* resource = (Resource<Texture>*)(*it).second;
			ImGui::Image((ImTextureID)resource->getContents(), ImVec2(10, 10));
			ImGui::SameLine();
			ImGui::Text(" ");
			it++;
		}
		ImGui::EndCombo();
	}
	ImGui::EndChild();
	ImGui::PopStyleVar(2);
}
