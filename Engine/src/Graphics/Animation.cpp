#include "Animation.hpp"

Animation::Animation(std::vector<Animation::Division> frames, int frameDuration)
{
	this->frameDuration = frameDuration;

	frames.resize(frames.size());
	for (int i = 0; i < frames.size(); i++)
		this->frames.push_back(frames[i]);
}

Animation::Animation(Resource<Texture>* texture, int dimX, int dimY, int frameDuration)
{
	this->frameDuration = frameDuration;

	if (texture->getContents() == nullptr)
		return;

	for (int i = dimY; i >= 0; i--)
	{
		for (int j = 0; j < dimX; j++)
		{
			float xmin = j * texture->getContents()->getWidth() / dimX;
			float ymin = i * texture->getContents()->getHeight() / dimY;
			float xmax = (j + 1) * texture->getContents()->getWidth() / dimX;
			float ymax = (i + 1) * texture->getContents()->getHeight() / dimY;

			frames.push_back(Division(xmin, ymin, xmax, ymax));
		}
	}
}

bool Animation::setFrame(int frameIndex, Animation::Division division)
{
	if (frameIndex < 0 || frameIndex >= frames.size())
		return false;
	else
		frames[frameIndex] = division;
}

Animation::Division Animation::getCurrFrame()
{
	return frames[currFrame];
}

void Animation::update()
{
	if (currTime++ >= frameDuration && frames.size()>0)
	{
		currFrame = (currFrame + 1) % frames.size();
		currTime = 0;
	}
}

void Animation::advance()
{
	if(frames.size() > 0)
		currFrame = (currFrame + 1) % frames.size();
}

std::vector<Animation::Division> Animation::divideEvenly(Resource<Texture>* texture, int dimX, int dimY)
{
	std::vector<Animation::Division> result;
	for (int i = dimY; i >= 0; i--)
	{
		for (int j = 0; j < dimX; j++)
		{
			float xmin = j * texture->getContents()->getWidth() / dimX;
			float ymin = i * texture->getContents()->getHeight() / dimY;
			float xmax = (j + 1) * texture->getContents()->getWidth() / dimX;
			float ymax = (i + 1) * texture->getContents()->getHeight() / dimY;

			result.push_back(Division(xmin, ymin, xmax, ymax));
		}
	}
	return result;
}

nlohmann::json Animation::serialize()
{
	nlohmann::json j;

	j["frameDuration"] = frameDuration;
	for (auto frame : frames)
	{
		j["frames"].push_back({ frame.xmin, frame.ymin, frame.xmax, frame.ymax });
	}

	return j;
}

Animation* Animation::deSerialize(nlohmann::json j)
{
	return nullptr;
}

Animation::Division::Division(float xmin, float ymin, float xmax, float ymax)
{
	this->xmin = xmin;
	this->ymin = ymin;
	this->xmax = xmax;
	this->ymax = ymax;
}
