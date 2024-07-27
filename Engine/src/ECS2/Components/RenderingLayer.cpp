#include "RenderingLayer.hpp"

nlohmann::json RenderingLayer::serialize()
{
    nlohmann::json j;
    j["type"] = CBase::RenderingLayer;

    j["layer"] = layer;

    return j;
}

void RenderingLayer::deSerialize(nlohmann::json j)
{
    layer = j["layer"];
}
