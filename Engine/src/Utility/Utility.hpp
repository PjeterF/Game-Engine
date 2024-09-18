#pragma once

#include <glm/glm.hpp>
#include <tuple>
#include <cmath>
#include <vector>

namespace utility
{
	namespace pairing
	{
		int mapIntegerToNatural(int a);
		int unMapNaturalToInteger(int a);

		int cantorPair(int a, int b);
		int cantorTriple(int a, int b, int c);

		int integerPair(int a, int b);
		int integerTriple(int a, int b, int c);

		glm::ivec2 undoCantorPair(int pair);
		glm::ivec2 undoIntegerPair(int n);
	}
	namespace sampling
	{
		std::vector<glm::vec4> sampleEvenly(int regionWidth, int regionHeight, int xOffset=0, int yOffset=0, int dimX=1, int dimY=1);
	}
	namespace tweens
	{
		float easeOutCubic(float t);
	}
	namespace random
	{
		float randomFloat(float min, float max);
		int randomInt(int min, int max);
	}
}