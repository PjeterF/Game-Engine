#pragma once

#include "SystemBase.hpp"
#include "../Components/CounterC.hpp"

class CounterKillerSystem : public SystemBase
{
public:
	static CounterKillerSystem& getInstance();

	virtual void to_json(nlohmann::json& j) const override;
	virtual void from_json(nlohmann::json& j) override;

	virtual void update(float dt) override;
private:
	CounterKillerSystem();
};