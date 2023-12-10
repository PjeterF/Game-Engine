#pragma once

#include <glm/vec2.hpp>

enum ObsBin
{
	Defualt = 0,
	ECS,
	UI
};

class Event
{
public:
	enum EventType
	{
		Nothing = 0,
		MouseMove, // glm::vec2 - mouse pos
		Shoot, //glm::vec2 - mouse position
		CtrlPointPlacement, //glm::vec2 - mouse position
		CtrlPointMovement, //glm::vec2 - mouse position
		ZoomChange, //float - zoom increment
		CameraPan, //glm::vec2
		RemoveLastRouteSegment,
		TogglePause,
		SelectObject,
		ObjectCollision, // Sprite* - array with collided entities
		GameOver,
		ShooterCreation, //Shooter* - shooter address
		ToggleCollisionGridVisibility,
		Reinitialize,
		CenterCamera,

		//ECS
		EntityRemoval, // Ent*
		ComponentRemoval, // Ent*
		MarbleInsertion, // Ent*

		//UI
		EntitySelection //Ent*
	};

	Event(EventType type, void* payload) : type(type), payload(payload) {}

	EventType getType() { return type; }
	void* getPayload() { return payload; }

private:
	void* payload;
	EventType type;
};

