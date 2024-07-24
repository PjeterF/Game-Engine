#pragma once

#include "CBase.hpp"
#include <unordered_set>

struct AABB : public CBase
{
	AABB(float width = 0, float height = 0, float mass=0, bool enabled = true, std::unordered_set<int> collidingEntIDs = {})
		: width(width), height(height), enabled(enabled), collidingEntIDs(collidingEntIDs), mass(mass) {}
	float width, height;
	float mass;
	bool enabled;
	std::unordered_set<int> collidingEntIDs;

	virtual nlohmann::json serialize() override;
	virtual void deSerialize(nlohmann::json j) override;
};