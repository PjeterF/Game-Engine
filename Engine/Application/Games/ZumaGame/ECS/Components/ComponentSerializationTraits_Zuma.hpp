#pragma once

#include "../../src/ECS2/Components/ComponentSerializationTraits.hpp"

#include "MarbleShooter.hpp"

template<>
struct ComponentSerializationTraits<MarbleShooter>
{
	static nlohmann::json serialize(MarbleShooter& c);
	static void deSerialize(MarbleShooter& c, nlohmann::json j);
};

inline nlohmann::json ComponentSerializationTraits<MarbleShooter>::serialize(MarbleShooter& c)
{
	nlohmann::json j;
	j["type"] = CBase::MarbleShooter;

	j["cooldown"] = c.cooldown;
	j["counter"] = c.counter;
	j["shotSpeed"] = c.shotSpeed;

	return j;
}

inline void ComponentSerializationTraits<MarbleShooter>::deSerialize(MarbleShooter& c, nlohmann::json j)
{
	c.cooldown = j["cooldown"];
	c.counter = j["counter"];
	c.shotSpeed = j["shotSpeed"];
}