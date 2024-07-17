#pragma once

#include "../../src/ECS2/Systems/SysBase.hpp"
#include"../../src/ECS2/Components/Counter.hpp"

class CounterKillerS : public SysBase
{
public:
	CounterKillerS();
	virtual void update(float dt) override;
};