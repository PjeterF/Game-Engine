#pragma once

#include "ComponentBase.hpp"

class CounterC : public ComponentBase
{
public:
	CounterC(int value = 0);

	virtual void to_json(nlohmann::json& j) const;
	virtual void from_json(nlohmann::json& j);

	int originalValue;
	int counter = 0;
};