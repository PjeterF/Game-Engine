#include "AIDirector.hpp"

AIDirectorS::AIDirectorS(int playerEntID)
{
	Entity ent = EntityManager::getInstance().getEntity(playerEntID);
	if (!ent.isValid())
		playerEntID = -1;

	if(!ent.hasComponent<Transform>())
		playerEntID = -1;

	this->playerEntID = playerEntID;
}

void AIDirectorS::update(float dt)
{
}
