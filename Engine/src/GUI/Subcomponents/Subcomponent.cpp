#include "Subcomponent.hpp"

Subcomponent::Subcomponent(std::string name)
{
	this->name = name;
}

std::string Subcomponent::getName()
{
	return name;
}

void Subcomponent::setName(std::string name)
{
	this->name = name;
}
