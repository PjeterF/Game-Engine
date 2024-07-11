#include "UIWindow.hpp"

bool UIWindow::isHovered()
{
	return hovered;
}

void UIWindow::windowConfig()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(width, height));
	ImGui::SetNextWindowPos(ImVec2(x, y));
	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	hovered = ImGui::IsWindowHovered();
}

void UIWindow::windowEnd()
{
	ImGui::End();
	ImGui::PopStyleVar(1);
}
