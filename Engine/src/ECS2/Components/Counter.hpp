#pragma once

#include "CBase.hpp"

struct Counter : public CBase
{
	Counter(int value = 0, int maxValue = 1) : val(value), maxValue(maxValue) {}
	int val;
	int maxValue;

	virtual nlohmann::json serialize() override;
	virtual void deSerialize(nlohmann::json j) override;
};