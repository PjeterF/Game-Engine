#pragma once

#include<typeindex>
#include<typeinfo>
#include<unordered_map>
#include<functional>

#include "../CBase.hpp"

namespace ComponentSerializer
{
	std::unordered_map<std::type_index, std::function<void(CBase* component)>> serializationFunctions;
	std::unordered_map<std::type_index, std::function<void(CBase* component)>> deSerializationFunctions;
}