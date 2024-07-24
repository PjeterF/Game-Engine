#pragma once

#include "CBase.hpp"

struct Transform : public CBase
{
	Transform(float x=0, float y=0, float width=0, float height=0, float rot=0, bool rotateToDir = false) : x(x), y(y), width(width), height(height), rot(rot), rotateToDir(rotateToDir) {}
	float x, y, width, height, rot;
	bool rotateToDir;

	virtual nlohmann::json serialize() override;
	virtual void deSerialize(nlohmann::json j) override;
};