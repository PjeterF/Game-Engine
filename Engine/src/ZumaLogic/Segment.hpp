#pragma once

#include "Sprite.hpp"
#include "../Graphics/Animation.hpp"

class Snake;
class SegmentTemplate;

class Segment : public Sprite, public Animation
{
public:
	Segment(float x, float y, float size, Resource<Texture>* texture, int layer=0);
	Segment(float x, float y, SegmentTemplate& segmentTemplate);
	virtual ~Segment();
	virtual void draw(RenderingAPI* renderer) override;
	virtual void update() override;
	virtual void onEntry(Collider* other);

	int targetSampleIndex = 1;
	int tag = -1;
	bool visible = false;
	bool moving = true;
	bool inserting = false;
	Snake* snake=nullptr;

	static std::vector<Resource<Texture>*> segmentTextures;
	static std::vector<SegmentTemplate> segmentTemplates;
	static int insetringDuration;
	static float insetringSpeed;
private:
	int counter = 0;
};

class SegmentTemplate : public Animation
{
public:
	SegmentTemplate(Resource<Texture>* texture, std::vector<Animation::Division> frames, int frameDuration, int size, int tag);
	SegmentTemplate(Resource<Texture>* texture, int dimX, int dimY, int frameDuration, int size, int tag);
	~SegmentTemplate();
	Resource<Texture>* texture;
	int size;
	int tag;

	nlohmann::json serialize();
	static SegmentTemplate* deserialize(nlohmann::json j);
};