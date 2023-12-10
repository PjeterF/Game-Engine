#pragma once

#include "SystemBase.hpp"
#include "../Components/TransformC.hpp"
#include "../Components/VelocityC.hpp"

class MovementSystem : public SystemBase
{
public:
	MovementSystem();
	virtual void handleEvent(Event& event) override;
	virtual void update(float dt) override;
};