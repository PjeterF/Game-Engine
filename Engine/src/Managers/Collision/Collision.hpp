#pragma once

#include "Collider.hpp"

class Collision
{
public:
	enum STATE
	{
		ENTER,
		STAY,
		EXIT,
		DELETE
	};

	Collision(Collider* col1, Collider* col2);
	
	Collider* col1;
	Collider* col2;
	STATE state;
	void update();
private:
};