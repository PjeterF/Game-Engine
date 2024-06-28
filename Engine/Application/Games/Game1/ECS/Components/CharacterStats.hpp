#pragma once

struct CharacterStats
{
	CharacterStats(float currenthealth = 0, float maxHealth = 0, float collisionDamage = 0, float attackDamage = 0, float speed = 0)
		: currenthealth(currenthealth), maxHealth(maxHealth), collisionDamage(collisionDamage), attackDamage(attackDamage), speed(speed)
	{}

	float currenthealth;
	float maxHealth;
	float collisionDamage;
	float attackDamage;
	float speed;
};