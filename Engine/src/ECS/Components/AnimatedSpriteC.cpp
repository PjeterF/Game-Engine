#include "AnimatedSpriteC.hpp"

TextureDivision::TextureDivision(float x, float y, float width, float height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

AnimatedSpriteC::AnimatedSpriteC(Resource<Texture>* texture, std::vector<TextureDivision> divisions, int frameDuration) : SpriteC(texture)
{
	this->divisions = divisions;
	this->frameDuration = frameDuration;
	this->type = AnimatedSprite;
}

void AnimatedSpriteC::to_json(nlohmann::json& j) const
{
	ComponentBase::to_json(j);
	SpriteC::to_json(j);
	j["frameDuration"] = frameDuration;
	j["currentFrame"] = currentFrame;
	j["divisions"] = nlohmann::json::array();
	j["paused"] = paused;

	for (auto& division : divisions)
	{
		j["divisions"].push_back({ division.x, division.y, division.width, division.height });
	}
}

void AnimatedSpriteC::from_json(nlohmann::json& j)
{
	SpriteC::from_json(j);

	this->frameDuration = j["frameDuration"];
	this->currentFrame = j["currentFrame"];
	this->paused = j["paused"];
	
	divisions.clear();
	for (auto division : j["divisions"])
		divisions.push_back(TextureDivision(division[0], division[1], division[2], division[3]));
}
