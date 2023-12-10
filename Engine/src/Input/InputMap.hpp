#pragma once

#include <unordered_map>

enum InputAction
{
	NoAction = 0
};

class InputMap
{
public:
	static InputMap& getInstance()
	{
		static InputMap map;
		return map;
	}

	void map(int keycode, InputAction action);
	void unmap(int keycode, InputAction action);
	const std::list<InputAction>* getActions(int keycode);
private:
	InputMap();
	std::unordered_map<int, std::list<InputAction>> mapping;
};