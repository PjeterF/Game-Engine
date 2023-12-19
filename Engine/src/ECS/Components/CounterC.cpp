#include "CounterC.hpp"

CounterC::CounterC(int value) : ComponentBase(Counter)
{
	this->originalValue = value;
}

void CounterC::to_json(nlohmann::json& j) const
{
	ComponentBase::to_json(j);
	j["originalValue"] = originalValue;
}

void CounterC::from_json(nlohmann::json& j)
{
	originalValue = j["originalValue"];
}
