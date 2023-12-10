#include "Segment.hpp"
#include "Snake.hpp"
#include <algorithm>

int Segment::insetringDuration = 10;
float Segment::insetringSpeed = 2;
std::vector<Resource<Texture>*> Segment::segmentTextures = {};
std::vector<SegmentTemplate> Segment::segmentTemplates = {};

Segment::Segment(float x, float y, float size, Resource<Texture>* texture, int layer) : Sprite(x, y, size, texture, layer), Animation({}, 0)
{
	type = segment;
}

Segment::Segment(float x, float y, SegmentTemplate& segmentTemplate) : Sprite(x, y, segmentTemplate.size, segmentTemplate.texture), Animation(segmentTemplate.frames, segmentTemplate.frameDuration)
{
	type = segment;
	this->tag = segmentTemplate.tag;
}

Segment::~Segment()
{
	if (snake != nullptr)
	{
		this->snake->segments.remove(this);
	}
}

void Segment::draw(RenderingAPI* renderer)
{
	if (visible)
	{
		if (frames.size() > 0)
		{
			renderer->spriteRenderer->draw(position, size, angle, texture->getContents(), this->getCurrFrame());
		}
		else
		{
			renderer->spriteRenderer->draw(position, size, angle, texture->getContents());
		}
	}
}

void Segment::update()
{
	if (snake != nullptr)
	{
		if (visible && inserting)
		{
			position = position + insetringSpeed * velocity * direction;
			Animation::update();
			if (counter++ == insetringDuration)
			{
				inserting = false;
				counter = 0;
			}
		}
		else if (visible && snake->moving && moving)
		{
			position = position + velocity * direction;
			Animation::update();
		}
	}
	else
	{
		if (moving)
		{
			position = position + velocity * direction;
			Animation::update();
		}
	}
	if(direction.x>0)
		angle = atan(direction.y / direction.x) * 57.2958;
	else
		angle = atan(direction.y / direction.x) * 57.2958+180;
	updateCollider(position.x, position.y, size.x, size.y);
}

void Segment::onEntry(Collider* other)
{
	Entity* p = dynamic_cast<Entity*>(other);
	if (p == nullptr)
	{
		return;
	}
	else
	{
		p = (Entity*)p;
	}
	switch (p->type)
	{
	case segment:
	{
		Segment* otherSeg = (Segment*)other;

		//if (this->pendingDelete || otherSeg->pendingDelete)
		//	return;

		// Inserting
		if (this->snake == nullptr && otherSeg->snake != nullptr)
		{
			auto it_other = std::find(otherSeg->snake->segments.begin(), otherSeg->snake->segments.end(), otherSeg);

			float forwardLen = glm::length(otherSeg->position + otherSeg->direction * otherSeg->size - this->position);
			float reverseLen = glm::length(otherSeg->position - otherSeg->direction * otherSeg->size - this->position);

			if (forwardLen < reverseLen)
			{
				this->targetSampleIndex = otherSeg->targetSampleIndex;
				this->velocity = otherSeg->snake->velocity;
				this->direction = glm::normalize(otherSeg->snake->spline->sampledPoints[otherSeg->targetSampleIndex] - this->position);
				this->inserting = true;
				this->snake = otherSeg->snake;
				otherSeg->snake->segments.insert(it_other, this);

				auto it_this = std::find(this->snake->segments.begin(), this->snake->segments.end(), this);
				if ((*it_this)->snake->checkPoppingCondition(it_this))
				{
					(*it_this)->snake->popSame(it_this);
				}
				else
				{
					auto it = this->snake->segments.begin();
					while (it != it_other)
					{
						(*it)->inserting = true;
						it++;
					}
				}
			}
			else
			{
				if(it_other != otherSeg->snake->segments.end())
					it_other++;
				this->targetSampleIndex = otherSeg->targetSampleIndex;
				this->velocity = otherSeg->snake->velocity;
				this->direction = glm::normalize(otherSeg->snake->spline->sampledPoints[otherSeg->targetSampleIndex] - this->position);
				this->inserting = true;
				this->snake = otherSeg->snake;
				otherSeg->snake->segments.insert(it_other, this);

				auto it_this = std::find(this->snake->segments.begin(), this->snake->segments.end(), this);
				if ((*it_this)->snake->checkPoppingCondition(it_this))
				{
					(*it_this)->snake->popSame(it_this);
				}
				else
				{
					auto it = this->snake->segments.begin();
					while (it != it_other)
					{
						(*it)->inserting = true;
						it++;
					}
				}
			}
			return;
		}

		// Merging
		if (this->snake != nullptr && otherSeg->snake!=nullptr && this->snake!=otherSeg->snake)
		{
			auto it_other = std::find(otherSeg->snake->segments.begin(), otherSeg->snake->segments.end(), otherSeg);
			auto it_this = std::find(this->snake->segments.begin(), this->snake->segments.end(), this);

			if (it_other == otherSeg->snake->segments.end() || it_this==this->snake->segments.end())
				return;

			float forwardTest = glm::length(this->position + this->direction*this->size - otherSeg->position);
			float reverseTest = glm::length(this->position - this->direction*this->size - otherSeg->position);

			if (forwardTest < reverseTest)
			{
				// this is behind
				(*it_other)->snake->moving = true;
				(*it_other)->snake->segments.splice((*it_other)->snake->segments.end(), (*it_this)->snake->segments);

				(*it_other)->snake->updateOwnership();
				(*it_this)->snake->updateOwnership();

				if ((*it_other)->snake->checkPoppingCondition(it_other))
				{
					(*it_other)->snake->popSame(it_other);
				}
			}
			else
			{
				// this is in front
				(*it_this)->snake->moving = true;
				(*it_this)->snake->segments.splice((*it_this)->snake->segments.end(), (*it_other)->snake->segments);

				(*it_other)->snake->updateOwnership();
				(*it_this)->snake->updateOwnership();

				if ((*it_this)->snake->checkPoppingCondition(it_this))
				{
					(*it_this)->snake->popSame(it_this);
				}
			}
			return;
		}
	}
		break;
	default:
		break;
	}
}

SegmentTemplate::SegmentTemplate(Resource<Texture>* texture, std::vector<Animation::Division> frames, int frameDuration, int size, int tag) : Animation(frames, frameDuration)
{
	this->texture = texture;
	texture->subscribe();
	this->size = size;
	this->frames = frames;
	this->frameDuration = frameDuration;
	this->tag = tag;
}

SegmentTemplate::SegmentTemplate(Resource<Texture>* texture, int dimX, int dimY, int frameDuration, int size, int tag) : Animation(texture, dimX, dimY, frameDuration)
{
	this->texture = texture;
	texture->subscribe();
	this->size = size;
	this->frameDuration = frameDuration;
	this->tag = tag;
}

SegmentTemplate::~SegmentTemplate()
{
	texture->unsubscribe();
}

nlohmann::json SegmentTemplate::serialize()
{
	nlohmann::json j = Animation::serialize();
	j["size"] = size;
	j["tag"] = tag;
	j["texture"] = texture->getContents()->getFilepath();

	std::cout << j.dump(4);

	return j;
}

SegmentTemplate* SegmentTemplate::deserialize(nlohmann::json j)
{
	std::vector<Animation::Division> frames;
	for (auto it = j["frames"].begin(); it != j["frames"].end(); it++)
	{
		frames.push_back(Animation::Division((*it)[0], (*it)[1], (*it)[2], (*it)[3]));
	}

	SegmentTemplate* newTemplate = new SegmentTemplate
	(
		ResourceManager::getInstance().getResource<Texture>(j["texture"]),
		frames,
		j["frameDuration"],
		j["size"],
		j["tag"]
	);

	return newTemplate;
}
