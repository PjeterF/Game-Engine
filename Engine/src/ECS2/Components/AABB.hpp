#pragma once

#include <vector>

struct AABB
{
	AABB(float width = 0, float height = 0, bool enabled = true, std::vector<int> collidingEntIDs = {}) : width(width), height(height), enabled(enabled), collidingEntIDs(collidingEntIDs) {}
	float width, height;
	bool enabled;
	std::vector<int> collidingEntIDs;
};