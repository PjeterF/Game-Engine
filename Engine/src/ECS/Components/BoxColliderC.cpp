#include "BoxColliderC.hpp"

BoxColliderC::BoxColliderC(float x, float y, float xSize, float ySize, Ent* parentEnt) : ComponentBase(CType::BoxCollider)
{
	this->colX = x;
	this->colY = y;
	this->colSizeX = xSize;
	this->colSizeY = ySize;
	this->parentEnt = parentEnt;
}

nlohmann::json BoxColliderC::serialize()
{
	nlohmann::json j = ComponentBase::serialize();

	j["position"] = { colX, colY };
	j["size"] = { colSizeX, colSizeY };

	return j;
}

void BoxColliderC::to_json(nlohmann::json& j) const
{
	ComponentBase::to_json(j);
	j["position"] = { colX, colY };
	j["size"] = { colSizeX, colSizeY };
}

void BoxColliderC::from_json(nlohmann::json& j)
{
	colX = j["position"][0];
	colY = j["position"][1];

	colSizeX = j["size"][0];
	colSizeY = j["size"][1];
}
