#include "Counter.hpp"

nlohmann::json Counter::serialize()
{
    nlohmann::json j;
    j["type"] = CBase::Counter;
    
    j["val"] = val;
    j["maxValue"] = maxValue;

    return j;
}

void Counter::deSerialize(nlohmann::json j)
{
    val = j["val"];
    maxValue = j["maxValue"];
}
