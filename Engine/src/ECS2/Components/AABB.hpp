#pragma once

#include <unordered_set>

struct AABB
{
	AABB(float width = 0, float height = 0, bool enabled = true, std::unordered_set<int> collidingEntIDs = {}) : width(width), height(height), enabled(enabled), collidingEntIDs(collidingEntIDs) {}
	float width, height;
	bool enabled;
	std::unordered_set<int> collidingEntIDs;
};