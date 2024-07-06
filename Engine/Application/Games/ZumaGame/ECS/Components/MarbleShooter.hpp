#pragma once

struct MarbleShooter
{
	MarbleShooter(float shotSpeed = 0, int cooldown = 0, int currentShotID = -1, int nextShotID = -1, int counter = 0)
		: shotSpeed(shotSpeed), cooldown(cooldown), currentShotID(currentShotID), nextShotID(nextShotID), counter(counter) {}

	int cooldown;
	int counter;
	int currentShotID, nextShotID;
	float shotSpeed;
};