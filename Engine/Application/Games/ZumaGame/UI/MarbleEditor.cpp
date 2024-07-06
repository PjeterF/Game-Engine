#include "MarbleEditor.hpp"

MarbleEditor::MarbleEditor(std::string name, int x, int y, int width, int height, std::string archetypeDirectoryPath)
	:UIWindow(name, x, y, width, height), archetypeDirectoryPath(archetypeDirectoryPath)
{
}

void MarbleEditor::render()
{
	prelude();

	ImGui::Text("Test");

	epilogue();
}
