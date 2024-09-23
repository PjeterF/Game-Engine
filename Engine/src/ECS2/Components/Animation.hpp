#pragma once

#include "CBase.hpp"

#include <vector>
#include <map>
#include <glm/glm.hpp>

struct Animation : public CBase
{
	enum State { idle = 0, walk = 1, run = 2};

	Animation(std::map<State, std::vector<glm::vec4>> framesMap = {}, State state = idle, float frameDuration = 1, int currentFrame = 0, float counter = 0)
		: currentFrame(currentFrame), frameDuration(frameDuration), framesMap(framesMap), counter(counter), state(state) {}

	int currentFrame;
	float frameDuration;
	std::map<State, std::vector<glm::vec4>> framesMap;
	float counter;
	State state;

	virtual nlohmann::json serialize() override;
	virtual void deSerialize(nlohmann::json j) override;
};