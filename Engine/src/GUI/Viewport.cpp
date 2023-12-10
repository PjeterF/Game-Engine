#include "Viewport.hpp"
#include "../ZumaLogic/Segment.hpp"
#include "../ZumaLogic/Shooter.hpp"
#include "../ZumaLogic/Snake.hpp"
#include "../ZumaLogic/Route.hpp"

Viewport::Viewport(int xmin, int ymin, int xmax, int ymax, unsigned int textureID, GLFWwindow* window, Camera* camera) : GUI_Window(xmin, ymin, xmax, ymax)
{
	this->textureID = textureID;
	this->window = window;
	this->camera = camera;
}
void Viewport::render()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(xmax-xmin, ymax-ymin));
	ImGui::SetNextWindowPos(ImVec2(xmin, ymin));
	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	ImGui::Image((ImTextureID)textureID, ImVec2(xmax - xmin, ymax - ymin), ImVec2(0, 0), ImVec2(1, -1));

	if (ImGui::BeginPopup("RouteEdit"))
	{
		if (selectedEntity != nullptr)
		{
			switch (selectedEntity->type)
			{
			case Entity::route:
			{
				Route* entity = (Route*)selectedEntity;
				if (ImGui::Selectable("Add segment"))
				{
					glm::vec2 mousePos = convertPos(io.MousePos.x, io.MousePos.y);
					entity->spline->addSegment(mousePos.x, mousePos.y);
				}
				if (ImGui::Selectable("Remove last segmant"))
				{
					entity->spline->removeLastSegment();
				}
			}
			break;
			default:
				break;
			}
		}
		ImGui::EndPopup();
	}


	if (ImGui::IsWindowHovered())
	{
		if (io.MouseClicked[1])
		{
			ImGui::OpenPopup("RouteEdit");
		}

		if (io.MouseWheel != 0)
		{
			camera->changeZoom(0.05 * io.MouseWheel);
		}

		if (io.MouseClicked[2])
		{
			panning = true;
			panAnchor = convertPos(io.MousePos.x, io.MousePos.y);
		}
		if (io.MouseReleased[2])
		{
			panning = false;
		}
		if (panning)
		{
			glm::vec2 pan = convertPos(io.MousePos.x, io.MousePos.y) - panAnchor;
			camera->pan(pan.x, pan.y);
		}

		if (mouseMoved())
		{
			glm::vec2 mousePos = convertPos(io.MousePos.x, io.MousePos.y);
			EventManager::getInstance().notify(Event(Event::MouseMove, &mousePos));
		}

		if (paused)
		{
			if (io.MouseClicked[0])
			{
				glm::vec2 mousePos = convertPos(io.MousePos.x, io.MousePos.y);
				selectedEntity=selectEntity(mousePos.x, mousePos.y);
				EventManager::getInstance().notify(Event(Event::CtrlPointMovement, &mousePos));
			}
		}
		else
		{
			if (io.MouseClicked[0])
			{
				glm::vec2 mousePos = convertPos(io.MousePos.x, io.MousePos.y);
				EventManager::getInstance().notify(Event(Event::Shoot, &mousePos));
			}
		}
	}

	ImGui::End();
	ImGui::PopStyleVar(2);
}

void Viewport::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::SelectObject:
	{
		selectedEntity = (Entity*)event.getPayload();
	}
	break;
	case Event::TogglePause:
	{
		paused = !paused;
	}
	break;
	default:
		break;
	}
}

glm::vec2 Viewport::convertPos(float x, float y)
{
	x = x - camera->getOffset().x;
	y = y + camera->getOffset().y;

	float xScale = (x - xmin) / (xmax-xmin);
	float yScale = (y - ymin) / (ymax-ymin);

	x = (xmax - xmin) * xScale / camera->getZoom();
	y = (ymax - ymin) * yScale / camera->getZoom();

	y = (ymax - ymin) / camera->getZoom() - y;

	return glm::vec2(x, y);
}

Entity* Viewport::selectEntity(float x, float y)
{
	float delta = 20;

	for (auto layer : EntityManager::getInstance().layers)
	{
		for (auto entry : layer->entities)
		{
			Entity* entity = entry.second;
			switch (entity->type)
			{
			case Entity::route:
			{
				Route* ent = (Route*)entity;
				for (auto ctrlPt : ent->spline->controlPoints)
				{
					bool xAxis = false;
					bool yAxis = false;

					if (x<ctrlPt.x + ent->spline->control_point_size && x>ctrlPt.x - ent->spline->control_point_size)
					{
						xAxis = true;
					}
					if (y<ctrlPt.y + ent->spline->control_point_size && y>ctrlPt.y - ent->spline->control_point_size)
					{
						yAxis = true;
					}

					if (xAxis && yAxis)
					{
						EventManager::getInstance().notify(Event(Event::SelectObject, ent));
						return ent;
					}
				}
			}
			break;
			case Entity::shooter:
			{
				Shooter* ent = (Shooter*)entity;
				if (ent->checkPointCollision(x, y))
				{
					EventManager::getInstance().notify(Event(Event::SelectObject, ent));
					return ent;
				}
			}
			break;
			case Entity::sprite:
			{
				Sprite* ent = (Sprite*)entity;
				if (ent->checkPointCollision(x, y))
				{
					EventManager::getInstance().notify(Event(Event::SelectObject, ent));
					return ent;
				}
			}
			break;
			case Entity::segment:
			{
				Segment* ent = (Segment*)entity;
				if (ent->checkPointCollision(x, y))
				{
					EventManager::getInstance().notify(Event(Event::SelectObject, ent));
					return ent;
				}
			}
			break;
			/*case Entity::snake:
			{
				Snake* ent = (Snake*)entity;
				for (auto ctrlPt : ent->spline->controlPoints)
				{
					bool xAxis = false;
					bool yAxis = false;

					if (x<ctrlPt.x + ent->spline->control_point_size && x>ctrlPt.x - ent->spline->control_point_size)
					{
						xAxis = true;
					}
					if (y<ctrlPt.y + ent->spline->control_point_size && y>ctrlPt.y - ent->spline->control_point_size)
					{
						yAxis = true;
					}

					if (xAxis && yAxis)
					{
						EventManager::getInstance().notify(Event(Event::SelectObject, ent));
						return ent;
					}
				}
			}*/
			break;
			default:
				break;
			}
		}
		return nullptr;
	}
} 

void Viewport::setTexture(unsigned int textureID)
{
	this->textureID = textureID;
}

bool Viewport::mouseMoved()
{
	ImGuiIO& io = ImGui::GetIO();

	if (prevMousePos.x != io.MousePos.x || prevMousePos.y != io.MousePos.y)
	{
		prevMousePos.x = io.MousePos.x;
		prevMousePos.y = io.MousePos.y;
		return true;
	}
	else
	{
		return false;
	}
}