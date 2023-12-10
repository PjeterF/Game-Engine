#include "RouteEdit.hpp"

RouteEdit::RouteEdit(std::string name, CubicBezierSpline* spline, Viewport* viewport) : Subcomponent(name)
{
	this->route = spline;
	this->viewport = viewport;
}

void RouteEdit::render()
{
	if (ImGui::Button("Add segment", ImVec2(200, 20)))
	{
		viewport->setState(Viewport::AddSegment);
	}
	if (ImGui::Button("Remove last segment", ImVec2(200, 20)))
	{
		route->removeLastSegment();
	}
	if (ImGui::Button("Move control points", ImVec2(200, 20)))
	{
		viewport->setState(Viewport::MoveCtlrPoints);
	}
}