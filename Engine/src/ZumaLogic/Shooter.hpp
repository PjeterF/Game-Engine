#pragma once

#include "Sprite.hpp"
#include "Segment.hpp"

class Shooter : public Sprite, public EventObserver
{
public:
	Shooter(int x, int y, int size, Resource<Texture>* texture, float shotVelocity, float shotSize);
	void lookAt(float x, float y);
	void shoot(float x, float y);
	void reroll();
	virtual void draw(RenderingAPI* renderer);
	virtual void update();
	virtual void handleEvent(Event& event);
	virtual nlohmann::json serialize();
	static Shooter* deSerialize(nlohmann::json j);

	std::list<Segment*> projectiles;
	float shotVelocity;
	float shotSize;

private:
	int currentProjectileType;
	int nextProjectileType;
	bool shootingEnabled = false;
};