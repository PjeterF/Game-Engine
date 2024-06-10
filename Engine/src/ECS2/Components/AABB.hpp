#pragma once

struct AABB
{
	AABB(float width = 0, float height = 0, bool enabled=true) : width(width), height(height), enabled(enabled) {}
	float width, height;
	bool enabled;
};