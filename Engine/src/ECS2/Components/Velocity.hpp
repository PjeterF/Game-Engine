#pragma once

struct Velocity
{
	Velocity(float x = 0, float y = 0, float ax = 0, float ay = 0) : x(x), y(y), ax(ax), ay(ay) {}
	float x, y;
	float ax, ay;
};