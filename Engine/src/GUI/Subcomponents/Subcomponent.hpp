#pragma once

#include <string>

class Subcomponent
{
public:
	Subcomponent(std::string name);
	virtual void render()=0;
	std::string getName();
	void setName(std::string name);
private:
	std::string name;
};