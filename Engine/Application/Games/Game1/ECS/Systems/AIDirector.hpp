#pragma once

#include "../../src/ECS2/Systems/SysBase.hpp"
#include "../../src/ECS2/Components/Transform.hpp"

class AIDirectorS : public SysBase
{
public:
	AIDirectorS(int playerEntID);
	virtual void update(float dt) override;
private:
	int playerEntID;
};