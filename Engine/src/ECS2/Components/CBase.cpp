#include "CBase.hpp"

nlohmann::json CBase::serialize()
{
    throw "Component serialization not implemented";
}

void CBase::deSerialize(nlohmann::json j)
{
    throw "Component deserialization not implemented";
}
