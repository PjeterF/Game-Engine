#pragma once

#include "ComponentBase.hpp"
#include "../Entity/Ent.hpp"

class BoxColliderC : public ComponentBase
{
public:
	BoxColliderC(float x = 0.0f, float y = 0.0f, float xSize = 0.0f, float ySize = 0.0f, Ent* parentEnt = nullptr);
	virtual nlohmann::json serialize() override;
	virtual void to_json(nlohmann::json& j) const;
	virtual void from_json(nlohmann::json& j);

	float colX, colY, colSizeX, colSizeY;
	Ent* parentEnt;
	int ID = -1;
	std::list<int> collisionsIDs;
};