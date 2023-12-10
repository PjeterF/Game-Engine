#include "InputMap.hpp"

void InputMap::map(int keycode, InputAction action)
{
	auto it = mapping.find(keycode);
	if (it == mapping.end())
	{
		mapping.insert({ keycode, std::list<InputAction>() });
		mapping[keycode].push_back(action);
	}
	else
	{
		(*it).second.push_back(action);
	}
}

void InputMap::unmap(int keycode, InputAction action)
{
	auto it = mapping.find(keycode);
	if (it != mapping.end())
		(*it).second.remove(action);
}

const std::list<InputAction>* InputMap::getActions(int keycode)
{
	auto it = mapping.find(keycode);
	if (it == mapping.end())
		return nullptr;
	else
		return &(*it).second;
}

InputMap::InputMap()
{

}