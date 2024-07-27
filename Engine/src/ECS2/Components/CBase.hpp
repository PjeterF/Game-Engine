#pragma once

#include <nlohmann/json.hpp>

struct CBase
{
	enum type
	{
		Transform = 0,
		Velocity = 1,
		Sprite = 2,
		RenderingLayer = 3,
		Counter = 4,
		Animation = 5,
		AABB = 6,
		MarbleShooter = 7,
	};

	virtual nlohmann::json serialize();
	virtual void deSerialize(nlohmann::json j);
};