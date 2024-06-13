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

		int cantorPair(int a, int b);
		int cantorTriple(int a, int b, int c);

		int integerPair(int a, int b);
		int integerTriple(int a, int b, int c);

		std::tuple<int, int> undoCantorPair(int pair);
	}
	namespace sampling
	{
		std::vector<glm::vec4> sampleEvenly(int regionWidth, int regionHeight, int xOffset=0, int yOffset=0, int dimX=1, int dimY=1);
	}
	namespace tweens
	{
		float easeOutCubic(float t);
	}
}