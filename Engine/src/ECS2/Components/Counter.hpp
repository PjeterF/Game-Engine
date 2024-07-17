#pragma once

struct Counter
{
	Counter(int value = 0, int maxValue = 1) : val(value), maxValue(maxValue) {}
	int val;
	int maxValue;
};