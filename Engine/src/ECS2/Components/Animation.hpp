#pragma once

#include <vector>
#include <map>
#include <glm/glm.hpp>

struct Animation
{
	enum State { idle = 0, walk = 1, run = 2};

	Animation(std::map<State, std::vector<glm::vec4>> framesMap = {}, State state = idle, int frameDuration = 1, int currentFrame = 0, int counter = 0)
		: currentFrame(currentFrame), frameDuration(frameDuration), framesMap(framesMap), counter(counter), state(state) {}
	int currentFrame;
	int frameDuration;
	std::vector<glm::vec4> frames;
	std::map<State, std::vector<glm::vec4>> framesMap;
	int counter;
	State state;
};