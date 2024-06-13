#pragma once

#include <vector>
#include <glm/glm.hpp>

struct Animation
{
	Animation(int currentFrame = 0, int frameDuration = 1, std::vector<glm::vec4> frames = {}, int counter = 0) : currentFrame(currentFrame), frameDuration(frameDuration), frames(frames), counter(counter) {}
	int currentFrame;
	int frameDuration;
	std::vector<glm::vec4> frames;
	int counter = 0;
};