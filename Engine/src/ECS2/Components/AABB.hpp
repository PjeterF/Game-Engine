#pragma once

#include <unordered_set>

struct AABB
{
	AABB(float width = 0, float height = 0, float mass=0, bool enabled = true, std::unordered_set<int> collidingEntIDs = {})
		: width(width), height(height), enabled(enabled), collidingEntIDs(collidingEntIDs), mass(mass) {}
	float width, height;
	float mass;
	bool enabled;
	std::unordered_set<int> collidingEntIDs;
};