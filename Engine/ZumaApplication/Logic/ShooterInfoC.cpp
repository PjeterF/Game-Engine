#include "ShooterInfoC.hpp"

ShooterC::ShooterC(float shotSpeed, int cooldown) : ComponentBase(ShooterInfo)
{
	this->shotSpeed = shotSpeed;
	this->cooldownTime = cooldown;
}