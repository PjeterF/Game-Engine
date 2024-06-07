#pragma once

struct AABB
{
	AABB(float width = 0, float height = 0) : width(width), height(height) {}
	float width, height;
};