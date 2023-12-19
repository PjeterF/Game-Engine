#pragma once

#include "glm/vec2.hpp"
#include "../../../Externals/includes/nlohmann/json.hpp"

#include <typeinfo>
#include <typeindex>

enum CType
{
	Transform,
	Velocity,
	Sprite,
	BoxCollider,
	AnimatedSprite,
	RenderingLayer,
	Particle,
	Counter,

	//Zuma
	RouteInfo,
	ShooterInfo
};

class ComponentBase
{
public:
	ComponentBase(int type);
	int getType();
	int entityID = -1;

	virtual void to_json(nlohmann::json& j) const;
	virtual void from_json(nlohmann::json& j);
protected:
	int type;
};