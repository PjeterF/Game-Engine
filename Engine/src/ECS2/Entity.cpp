#include "Entity.hpp"

Entity::Entity(int ID) : ID(ID)
{
	if (ID == -1)
		valid = false;
	else
		valid = true;
}

int Entity::getID()
{
	return ID;
}

bool Entity::isValid()
{
	return valid;
}
