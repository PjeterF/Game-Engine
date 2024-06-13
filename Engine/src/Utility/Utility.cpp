#include "Utility.hpp"

namespace utility
{
    namespace pairing
    {
        int mapIntegerToNatural(int a)
        {
            if (a >= 0)
                return 2 * a;
            else
                return -2 * a - 1;
        }

        int cantorPair(int a, int b)
        {
            return (a + b) * (a + b + 1) / 2 + b;
        }

        int cantorTriple(int a, int b, int c)
        {
            return cantorPair(cantorPair(a, b), c);
        }

        int integerPair(int a, int b)
        {
            return cantorPair(mapIntegerToNatural(a), mapIntegerToNatural(b));
        }

        int integerTriple(int a, int b, int c)
        {
            return integerPair(integerPair(a, b), c);
        }

        std::tuple<int, int> undoCantorPair(int pair)
        {
            int w = (int)((sqrt(8 * pair + 1) - 1) / 2);
            int t = (w * w + w) / 2;
            int y = pair - t;
            int x = w - y;
            return { x, y };
        }
    }
}

std::vector<glm::vec4> utility::sampling::sampleEvenly(int regionWidth, int regionHeight, int xOffset, int yOffset, int dimX, int dimY)
{
    std::vector<glm::vec4> result;
    glm::vec2 partitionDim = { regionWidth / dimX, regionHeight / dimY };
    for (int y = 0; y < dimY; y++) {
        for (int x = 0; x < dimX; x++)
        {
            float xPos = partitionDim.x * x + xOffset;
            float yPos = regionHeight - partitionDim.y * (y + 1) - yOffset;
            result.push_back({ xPos, yPos, partitionDim.x, partitionDim.y });
        }
    }
    return result;
}

float utility::tweens::easeOutCubic(float t)
{
    return 1 - pow(1.0f - t, 3);
}
