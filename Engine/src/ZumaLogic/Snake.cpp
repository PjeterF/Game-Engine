#include "Snake.hpp"
#include "Shooter.hpp"
#include "ShortLivedParticleEmitter.hpp"

Snake::Snake(int initalSegmentNumber, float segmentSize, int maxSameSegments, float velocity, CubicBezierSpline* spline)
{
	type = snake;
	this->spline = spline;
	this->velocity = velocity;
	this->initialSegmentNumber = initalSegmentNumber;
	this->segmentSize = segmentSize;
	this->maxSameSegments = maxSameSegments;
	this->segmentsToSpawn = initialSegmentNumber;
	
	if (Segment::segmentTemplates.size() == 0)
		return;

	int n = 0;
	int prev_random = -1;
	for (int i = 0; i < initalSegmentNumber; i++)
	{
		int random = rand() % Segment::segmentTemplates.size();
		n++;
		if (prev_random == random)
		{
			n++;
		}
		else
		{
			n = 0;
		}

		if (n >= maxSameSegments)
		{
			while (prev_random == random && Segment::segmentTemplates.size()>1)
			{
				random = rand() % Segment::segmentTemplates.size();
			}
		}

		prev_random = random;

		Segment* seg = new Segment(spline->controlPoints[0].x, spline->controlPoints[0].y, Segment::segmentTemplates[random]);
		seg->tag = Segment::segmentTemplates[random].tag;
		seg->velocity = velocity;
		seg->snake = this;
		seg->collision = false;

		segments.push_back(seg);
	}
	auto it = segments.begin();
	if (it != segments.end())
		makeVisible((*it));
}

Snake::~Snake()
{
	for (auto segment : segments)
	{
		segment->destroy();
		segment->snake = nullptr;
	}
}

void Snake::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::GameOver:
	{
		this->destroy();
		ParticeEmitter::ParticleProperties props;
		props.startColour = glm::vec4(0.1, 0.1, 0.9, 1);
		props.endColour = glm::vec4(0.1, 0.9, 0.9, 1);
		props.xVelVar = glm::vec2(-2, 2);
		props.yVelVar = glm::vec2(-2, 2);
		props.xPosVar = glm::vec2(-10, 10);
		props.yPosVar = glm::vec2(-10, 10);
		props.velocityDecay = 0.97;
		for (auto segment : segments)
		{
			if(segment->visible)
				ParticleBurst* explosion = new ParticleBurst(segment->position.x, segment->position.y, 10, 1000, props);
		}
	}
	break;
	default:
		break;
	}
}

void Snake::draw(RenderingAPI* renderer)
{
	
}

void Snake::update()
{
	if (segments.empty() || spline->controlPoints.size()<=1)
	{
		this->destroy();
		return;
	}

	auto it = segments.begin();
	float delta = 20;

	auto segPos = (*it)->position;
	auto samplePos = spline->sampledPoints[spline->sampledPoints.size()-1];
	if (glm::length(segPos - samplePos) < delta)
	{
		EventManager::getInstance().notify(Event(Event::GameOver, nullptr));
		return;
	}

	while (it != segments.end())
	{
		if (!(*it)->visible)
		{
			// Check whether segment should be made visible
			auto it_prev = it;
			if(it_prev!=segments.begin())
				it_prev--;
			if ((glm::length((*it_prev)->position - spline->controlPoints[0]) > (*it)->size.x*2))
			{
				makeVisible((*it));
			}
			else
			{
				break;
		}
	}


		// Check if current segment has reached its sample
		if ((*it)->targetSampleIndex < spline->sampledPoints.size())
		{
			auto segPos = (*it)->position;
			auto samplePos = spline->sampledPoints[(*it)->targetSampleIndex];
			if (glm::length(segPos - samplePos) < delta)
			{
				(*it)->targetSampleIndex++;
				if ((*it)->targetSampleIndex < spline->sampledPoints.size())
				{
					samplePos = spline->sampledPoints[(*it)->targetSampleIndex];
					(*it)->direction = glm::normalize(samplePos - (*it)->position);
				}
			}
		}


		// Check if current segment is far enough form the next one to move
		if (it != segments.begin())
		{
			auto it_prev = it;
			it_prev--;
			if (glm::length((*it)->position - (*it_prev)->position) > (*it)->size.x + (*it_prev)->size.x)
			{
				(*it)->moving = true;
			}
			else
			{
				(*it)->moving = false;
			}
		}
		else
		{
			(*it)->moving = true;
		}

		it++;
	}

	if (inserting)
	{
		this->moving = true;
		if (counter++ == this->insertingDuration)
		{
			counter = 0;
			inserting = false;
		}
	}
}

// Creates a new snake with segments from 'segment' to end
Snake* Snake::splice(std::list<Segment*>::iterator segment)
{
	Snake* newSnake = new Snake();
	newSnake->type = snake;
	newSnake->spline = this->spline;
	newSnake->velocity = this->velocity;

	newSnake->segments.splice(newSnake->segments.begin(), this->segments, segment, this->segments.end());
	newSnake->updateOwnership();

	return newSnake;
}

void Snake::updateOwnership()
{
	for (auto segment : segments)
	{
		segment->snake = this;
	}
}

nlohmann::json Snake::serialize()
{
	using json = nlohmann::json;

	json j;

	j["InitialSegmentCount"] = initialSegmentNumber;
	j["SegmentSize"] = segmentSize;
	j["MaxSameSegments"] = maxSameSegments;
	j["Velocity"] = velocity;

	return j;
}

Snake* Snake::deSerialize(nlohmann::json j, CubicBezierSpline* spline)
{
	int InitialSegmentCount = j["InitialSegmentCount"];
	int MaxSameSegments = j["MaxSameSegments"];
	int SegmentSize = j["SegmentSize"];
	int Velocity = j["Velocity"];

	return new Snake(InitialSegmentCount, SegmentSize, MaxSameSegments, Velocity, spline);
}

void Snake::updateSegmentsLayer()
{
	for (auto segment : segments)
	{
		EntityManager::getInstance().moveEntityToLayer(segment->Entity::getID(), this->layer);
	}
}

bool Snake::setLayer(int targetLayer)
{
	if (Entity::setLayer(targetLayer))
	{
		for (auto segment : segments)
		{
			segment->setLayer(targetLayer);
		}
	}
	else
	{
		return false;
	}
}

void Snake::makeVisible(Segment* segment)
{
	segment->visible = true;
	segment->collision = true;
	segment->position = spline->controlPoints[0];
	segment->direction = glm::normalize(spline->sampledPoints[1] - spline->controlPoints[0]);
}

void Snake::spawnSegment()
{
	segmentsToSpawn--;

	static int n = 0;
	static int prevRandom = -1;
	int random = rand() % Segment::segmentTextures.size();

	if (prevRandom == random)
		n++;
	else
		n = 0;

	if (n >= maxSameSegments)
	{
		prevRandom = random;
		n = 0;
		while (random == prevRandom)
		{
			random = rand() % Segment::segmentTextures.size();
		}
	}

	Segment* newSegment = new Segment(spline->controlPoints[0].x, spline->controlPoints[0].y, segmentSize, Segment::segmentTextures[random], this->layer);
	newSegment->tag = random;
	newSegment->velocity = velocity;
	newSegment->snake = this;
	newSegment->collision = true;
	newSegment->direction = glm::normalize(spline->sampledPoints[1] - spline->controlPoints[0]);

	segments.push_back(newSegment);
}

bool Snake::checkPoppingCondition(std::list<Segment*>::iterator segment)
{
	auto forward = segment;
	auto reverse = segment;
	bool same = true;
	int count = 0;

	if (reverse != this->segments.begin() && reverse != this->segments.end())
	{
		reverse--;
		same = false;
	}

	while (forward != this->segments.end())
	{
		if ((*forward)->tag == (*segment)->tag)
		{
			count++;
			forward++;
		}
		else
		{
			break;
		}
	}

	if (same)
	{
		if (count >= popingCount)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	while (reverse != this->segments.begin())
	{
		if ((*reverse)->tag == (*segment)->tag)
		{
			count++;
			reverse--;
		}
		else
		{
			break;
		}
	}

	if (reverse == this->segments.begin())
	{
		if ((*reverse)->tag == (*segment)->tag)
		{
			count++;
		}
	}

	if (count >= popingCount)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Snake::popSame(std::list<Segment*>::iterator segment)
{
	auto forward = segment;
	auto reverse = segment;
	bool same = true;
	std::list<Segment*>* segs = &this->segments;
	int tag = (*segment)->tag;

	if (reverse != segs->begin() && reverse != this->segments.end())
	{
		reverse--;
		same = false;
	}

	while (forward != segs->end())
	{
		if ((*forward)->tag == tag)
		{
			auto temp = forward;
			temp++;
			new ParticleBurst((*forward)->position.x, (*forward)->position.y, 50, 200);
			//delete (*forward);
			(*forward)->destroy();
			forward = temp;
		}
		else
		{
			break;
		}
	}

	if (same)
	{
		return;
	}

	while (reverse != segs->begin())
	{
		if ((*reverse)->tag == tag)
		{
			auto temp = reverse;
			temp--;
			new ParticleBurst((*reverse)->position.x, (*reverse)->position.y, 50, 200);
			//delete (*reverse);
			(*reverse)->destroy();
			reverse = temp;
		}
		else
		{
			break;
		}
	}

	if (reverse == segs->begin())
	{
		if ((*reverse)->tag == tag)
		{
			auto temp = reverse;
			temp++;
			new ParticleBurst((*reverse)->position.x, (*reverse)->position.y, 50, 200);
			//delete (*reverse);
			(*reverse)->destroy();
			reverse = temp;
		}
	}

	Snake* spliced = this->splice(forward);
	spliced->moving = false;
	this->updateOwnership();
	spliced->updateOwnership();

	if (this->moving)
	{
		spliced->moving = true;
		this->moving = false;
	}

	if (this->segments.size() == 0)
	{
		this->destroy();
		spliced->moving = false;
	}

	if (spliced->segments.size() == 0)
	{
		spliced->destroy();
		this->moving = false;
	}
}
