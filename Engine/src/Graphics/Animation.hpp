#pragma once

#include "../Managers/Resource/ResourceManager.hpp"

#include <nlohmann/json.hpp>

#include <vector>
#include <tuple>

class Animation
{
public:
	struct Division
	{
		Division(float xmin = 0, float ymin = 0, float xmax = 1, float ymax = 1);
		float xmin;
		float ymin;
		float xmax;
		float ymax;
	};

	Animation(std::vector<Animation::Division> frames, int frameDuration);
	Animation(Resource<Texture>* texture, int dimX, int dimY, int frameDuration);
	bool setFrame(int frame, Animation::Division division);
	Animation::Division getCurrFrame();
	void update();
	void advance();

 	static std::vector<Animation::Division> divideEvenly(Resource<Texture>* texture, int dimX, int dimY);

	virtual nlohmann::json serialize();
	static Animation* deSerialize(nlohmann::json j);

	std::vector<Animation::Division> frames;
	int frameDuration;
private:
	int currFrame = 0;
	int currTime = 0;
};