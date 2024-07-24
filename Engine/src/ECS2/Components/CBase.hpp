#pragma once

#include <nlohmann/json.hpp>

struct CBase
{
	virtual nlohmann::json serialize();
	virtual void deSerialize(nlohmann::json j);
};