#include "Collision.hpp"

Collision::Collision(Collider* col1, Collider* col2)
{
	this->col1 = col1;
	this->col2 = col2;
	state = ENTER;
}

void Collision::update()
{
	switch (state)
	{
	case Collision::ENTER:
	{
		state = STAY;
		col1->onEntry(col2);
		if (this->state == DELETE)
			return;
		col2->onEntry(col1);
		break;
	}
	case Collision::STAY:
	{
		if (col1->checkCollision(col2))
		{
			col1->onStay(col2);
			if (this->state == DELETE)
				return;
			col2->onStay(col1);
		}
		else
		{
			state = EXIT;
			col1->onExit(col2);
			if (this->state == DELETE)
				return;
			col2->onExit(col1);
			break;
		}
	case Collision::DELETE:
	{
		return;
		break;
	}
	}
	}
}

