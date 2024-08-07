#pragma once

#include "CBase.hpp"

struct Velocity : public CBase
{
	Velocity(float x = 0, float y = 0, float ax = 0, float ay = 0, float decay = 1) : x(x), y(y), ax(ax), ay(ay), decay(decay) {}
	float x, y;
	float ax, ay;
	float decay;

	virtual nlohmann::json serialize() override;
	virtual void deSerialize(nlohmann::json j) override;
};
