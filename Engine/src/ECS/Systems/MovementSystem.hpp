#pragma once

#include "SystemBase.hpp"
#include "../Components/TransformC.hpp"
#include "../Components/VelocityC.hpp"

class MovementSystem : public SystemBase
{
public:
	static MovementSystem& getInstance();

	virtual void to_json(nlohmann::json& j) const override;
	virtual void from_json(nlohmann::json& j) override;

	virtual void handleEvent(Event& event) override;
	virtual void update(float dt) override;
private:
	MovementSystem();
};