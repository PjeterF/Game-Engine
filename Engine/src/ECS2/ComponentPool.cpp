#include "ComponentPool.hpp"

bool ComponentPoolBase::has(int ID)
{
	if (sparse[ID] != -1)
		return true;
	else
		return false;
}

nlohmann::json ComponentPoolBase::serialize(int ID)
{
    throw "Serialization not implemented";
}

void ComponentPoolBase::deSerialize(int ID, nlohmann::json j)
{
    throw "Deserialization not implemented";
}
