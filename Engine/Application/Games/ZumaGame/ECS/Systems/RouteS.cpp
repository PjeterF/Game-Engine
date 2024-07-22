#include "RouteS.hpp"

#include "../../functions.hpp"

#include "../../src/ECS2/SystemsManager.hpp"
#include "../../src/ECS2/Systems/ParticleS.hpp"
#include "../../ECS/Systems/CounterKillerS.hpp"

RouteS::RouteS(std::string systemID, std::vector<glm::vec2> ctrlPts, unsigned int nSamples, float speed, int nMarbles, bool deleteOnSceneEnd)
	: ctrlPts(ctrlPts), nSamples(nSamples), remainingMarbles(nSamples), speed(speed), nMarbles(nMarbles), systemID(systemID), SysBase(deleteOnSceneEnd)
{
	requiredComponents = {
		std::type_index(typeid(Transform)),
		std::type_index(typeid(Velocity)),
		std::type_index(typeid(MarbleComponent)),
		std::type_index(typeid(AABB))
	};

	calculateIntermediatePoints();
} 

RouteS::RouteS(nlohmann::json jRoute)
{
	speed = jRoute["speed"];
	popThreshold = jRoute["popThreshold"];
	distanceBetween = jRoute["distanceBetween"];
	nMarbles = jRoute["nMarbles"];
	nSamples = jRoute["nSamples"];
	systemID = jRoute["systemID"];

	for (int i = 0; i < jRoute["ctrlPts"].size(); i++)
		ctrlPts.push_back({ jRoute["ctrlPts"][i][0], jRoute["ctrlPts"][i][1] });
}

RouteS::~RouteS()
{
	for (auto marble : marbles)
		EntityManager::getInstance().deleteEntity(marble);
}

void RouteS::handleEvent(Event& event)
{
	SysBase::handleEvent(event);
	switch (event.getType())
	{
	case Event::EntityRemoval:
	{
		int* ID = (int*)event.getPayload();
		
		for (auto it = marbles.begin(); it != marbles.end(); it++)
		{
			if (*it == *ID)
			{
				marbles.erase(it);
				break;
			}
		}
	}
	case Event::RouteSelection:
	{
		std::string* routeName = (std::string*)event.getPayload();

		if (*routeName == this->systemID)
			highlight = true;
		else
			highlight = false;
	}
	break;
	}
}

void RouteS::update(float dt)
{
	auto transPool = ComponentPoolManager::getInstance().getPool<Transform>();
	auto velPool = ComponentPoolManager::getInstance().getPool<Velocity>();
	auto mCompPool = ComponentPoolManager::getInstance().getPool<MarbleComponent>();
	auto colPool = ComponentPoolManager::getInstance().getPool<AABB>();

	float delta = 10;
	glm::vec2 distanceDelta = {20, 40};

	if (!marbles.empty())
		mCompPool->get(marbles.front()).separated = false;

	for (auto it = marbles.begin(); it != marbles.end(); it++)
	{
		int marbleID = *it;

		auto& transform = transPool->get(marbleID);
		auto& velocity = velPool->get(marbleID);
		auto& mComponent = mCompPool->get(marbleID);
		auto& collider = colPool->get(marbleID);

		auto itPrev = it;
		auto itNext = it;
		float nextDistance = distanceBetween + 1, prevDistance = distanceBetween - 1;

		itNext++;
		if (itNext != marbles.end()) {
			auto& transform2 = transPool->get(*itNext);
			auto& velocity2 = velPool->get(*itNext);
			auto& mComponent2 = mCompPool->get(*itNext);
			auto& collider2 = colPool->get(*itNext);

			glm::vec2 diff = { transform.x - transform2.x, transform.y - transform2.y };
			nextDistance = glm::length(diff);
		}

		bool shouldMerge = false;

		if (it != marbles.begin())
		{
			itPrev--;
			auto& transform2 = transPool->get(*itPrev);
			auto& velocity2 = velPool->get(*itPrev);
			auto& mComponent2 = mCompPool->get(*itPrev);
			auto& collider2 = colPool->get(*itPrev);

			glm::vec2 diff = { transform.x - transform2.x, transform.y - transform2.y };
			prevDistance = glm::length(diff);

			if (mCompPool->get(*itPrev).separated == false && mComponent.separated == true)
				shouldMerge = true;
		}

		if (prevDistance > distanceBetween || nextDistance < distanceBetween * 0.9)
		{
			velocity.x = 0;
			velocity.y = 0;
		}
		else
		{
			if (shouldMerge)
			{
				if (popSame(it))
					return;
			}

			if (intermediatePts.empty())
				return;
			if (mComponent.targetPointIdx < 0)
				mComponent.targetPointIdx = 0;
			if(mComponent.targetPointIdx >= intermediatePts.size())
				mComponent.targetPointIdx = intermediatePts.size()-1;

			glm::vec2 targetP = intermediatePts[mComponent.targetPointIdx];
			glm::vec2 dir = { targetP.x - transform.x, targetP.y - transform.y };

			if (glm::length(dir) > delta)
			{
				dir = glm::normalize(dir);
			}
			else
			{
				mComponent.targetPointIdx++;
				continue;
			}

			dir = speed * dir;

			velocity.x = dir.x;
			velocity.y = dir.y;
		}
	}
}

void RouteS::draw(RenderingAPI* rAPI)
{
	for (int i = 0; i < ctrlPts.size(); i++)
	{
		glm::vec4 color = {0.5, 1, 1, 1};
		if (i % 3 == 0)
			color *= 0.88;
		if (i % 3 == 1)
			color *= 0.67;
		if (i % 3 == 2)
			color *= 0.5;
		color.w = 1;
		rAPI->addQuadInstance(ctrlPts[i], { 5, 5 }, 0, color);
	}
	rAPI->drawQuadInstances();

	if (intermediatePts.empty())
		return;

	glm::vec3 lineColor = { 0.33, 0.11, 0.11 };
	if (highlight)
		lineColor = 3.0f * lineColor;
	for (int i = 0; i < intermediatePts.size() - 1; i++)
	{
		rAPI->drawLine(intermediatePts[i], intermediatePts[i + 1], 5, lineColor);
	}
}

void RouteS::calculateIntermediatePoints()
{
	if (ctrlPts.size() < 4)
		return;

	std::vector<float> tValues;
	for (float i = 0; i <= nSamples; i+=1)
	{
		tValues.push_back(i / nSamples);
	}

	intermediatePts.clear();
	int nSegments = (ctrlPts.size() - 1)/3;
	for (int i = 0; i < nSegments; i++)
	{
		glm::vec2 p0 = ctrlPts[i*3 + 0], p1 = ctrlPts[i*3 + 1], p2 = ctrlPts[i*3 + 2], p3 = ctrlPts[i*3 + 3];

		for (int j = 0; j < tValues.size(); j++)
		{
			float t = tValues[j];
			glm::vec2 point=
				(1 - t) * (1 - t)* (1 - t) * p0+
				3 *(1 - t)* (1 - t) * t * p1+
				3 * (1 - t) * t * t * p2+
				t * t * t * p3;

			intermediatePts.push_back(point);
		}
	}
}

void RouteS::setNSamples(unsigned int nSamples)
{
	this->nSamples = nSamples;
	calculateIntermediatePoints();
}

void RouteS::addSegment(glm::vec2 endPoint)
{
	if (ctrlPts.empty())
	{
		ctrlPts.push_back(endPoint);
		calculateIntermediatePoints();
		return;
	}
	
	glm::vec2 distanceVec = endPoint - ctrlPts.back();
	glm::vec2 segmentBegin = ctrlPts.back();

	ctrlPts.push_back(segmentBegin + 0.33f * distanceVec);
	ctrlPts.push_back(segmentBegin + 0.66f * distanceVec);
	ctrlPts.push_back(endPoint);

	calculateIntermediatePoints();
}

void RouteS::removeLastSegment()
{
	if (ctrlPts.size() == 1)
	{
		ctrlPts.clear();
		calculateIntermediatePoints();
		return;
	}

	ctrlPts.pop_back();
	ctrlPts.pop_back();
	ctrlPts.pop_back();

	calculateIntermediatePoints();
}

void RouteS::spawnMarbleAtOrigin(std::string marbleArchetypeFilepath)
{
	if (ctrlPts.empty())
		return;

	int marbleID = ZumaFn::spawnMarble(marbleArchetypeFilepath, ctrlPts[0], systemID);
	if (marbleID < 0)
		return;

	Entity marble(marbleID);

	if (!intermediatePts.empty())
		marble.getComponent<MarbleComponent>().targetPointIdx = 0;

	SystemsManager::getInstance().getSystem<MovementS>()->addEntity(marbleID);
	SystemsManager::getInstance().getSystem<RenderingS>()->addEntity(marbleID);
	SystemsManager::getInstance().getSystem<CollisionS>()->addEntity(marbleID);
	SystemsManager::getInstance().getSystem<AnimationS>()->addEntity(marbleID);
	this->addEntity(marbleID);

	marbles.push_front(marbleID);
}

void RouteS::spawnMarbleIfPossible(std::string marbleArchetypeFilepath)
{
	if (ctrlPts.empty() || remainingMarbles<=0)
		return;

	if (marbles.empty())
	{
		remainingMarbles--;
		spawnMarbleAtOrigin(marbleArchetypeFilepath);
		return;
	}

	Entity firstMarble(marbles.front());
	auto& transform = firstMarble.getComponent<Transform>();
		
	if (glm::length(glm::vec2(transform.x - ctrlPts[0].x, transform.y - ctrlPts[0].y))>distanceBetween*0.9)
	{
		remainingMarbles--;
		spawnMarbleAtOrigin(marbleArchetypeFilepath);
		return;
	}
}

void RouteS::setControlPointPos(int index, glm::vec2 newPos)
{
	if (index < 0 || index >= ctrlPts.size())
		return;

	ctrlPts[index] = newPos;
	calculateIntermediatePoints();
}

const std::string& RouteS::getID()
{
	return systemID;
}

bool RouteS::insertAt(int entID, int inserteeID)
{
	auto transformPool = ComponentPoolManager::getInstance().getPool<Transform>();
	auto velocityPool = ComponentPoolManager::getInstance().getPool<Velocity>();
	auto marbleCPool = ComponentPoolManager::getInstance().getPool<MarbleComponent>();

	for (auto it = marbles.begin(); it != marbles.end(); it++)
	{
		if (*it == entID)
		{
			if (addEntity(inserteeID))
			{
				marbleCPool->get(inserteeID).targetPointIdx = marbleCPool->get(*it).targetPointIdx;
				marbleCPool->get(inserteeID).routeName = systemID;

				auto& inserteeTransform = transformPool->get(inserteeID);
				auto& itTransform = transformPool->get(*it);

				inserteeTransform.x = itTransform.x;
				inserteeTransform.y = itTransform.y;

				auto itNext = it;
				itNext++;
				glm::vec2 currentPos = { itTransform.x, itTransform.y };
				glm::vec2 nextPos = currentPos;
				glm::vec2 inserteePos = { inserteeTransform.x, inserteeTransform.y };
				if (itNext != marbles.end())
				{
					auto& nextTransform = velocityPool->get(*itNext);
					nextPos = { nextTransform.x, nextTransform.y };
				}

				if (glm::distance(currentPos, inserteePos) <= glm::distance(nextPos, inserteePos))
					it++;

				if (it != marbles.end())
				{
					auto& itTrans = transformPool->get(*it);

					inserteeTransform.x = itTrans.x;
					inserteeTransform.y = itTrans.y;
				}

				it = marbles.insert(it, inserteeID);
				if (popSame(it))
					return true;
				it++;


				while (it != marbles.end())
				{
					auto& vel = velocityPool->get(*it);
					marbleCPool->get(*it).targetPointIdx++;
					glm::vec2 dir = glm::normalize(glm::vec2(vel.x, vel.y));

					vel.x = distanceBetween*dir.x;
					vel.y = distanceBetween*dir.y;

					it++;
				}

				return true;
			}

			return false;
		}
	}

	return false;
}

void RouteS::reset()
{
	for (int entID : marbles)
		EntityManager::getInstance().deleteEntity(entID);

	remainingMarbles = nMarbles;
}

void RouteS::setNumberOfMarbles(int n)
{
	nMarbles = n;
}

int RouteS::ctrlPointIntersection(glm::vec2 pos)
{
	int delta = 50;

	for(int i=0;i<ctrlPts.size();i++)
	{
		if (pos.x > ctrlPts[i].x - delta && pos.x < ctrlPts[i].x + delta &&
			pos.y > ctrlPts[i].y - delta && pos.y < ctrlPts[i].y + delta)
		{
			return i;
		}
	}

	return -1;
}

nlohmann::json RouteS::serialize()
{
	nlohmann::json j;

	j["speed"] = speed;
	j["popThreshold"] = popThreshold;
	j["distanceBetween"] = distanceBetween;
	j["nMarbles"] = nMarbles;
	j["nSamples"] = nSamples;
	j["systemID"] = systemID;

	j["ctrlPts"] = nlohmann::json::array();
	for (auto& point : ctrlPts)
		j["ctrlPts"].push_back({ point.x, point.y });

	return j;
}

bool RouteS::popSame(std::list<int>::iterator it)
{
	if (marbles.empty())
		return false;
	if (it == marbles.end())
		return false;

	auto marbleCPool = ComponentPoolManager::getInstance().getPool<MarbleComponent>();

	int type = marbleCPool->get(*it).type;
	int count = 0;

	auto itForwards = it;
	itForwards++;
	auto itBackwards = it;

	while (itForwards != marbles.end())
	{
		if (marbleCPool->get(*itForwards).type != type)
			break;

		count++;
			
		itForwards++;
	}

	while (1)
	{
		if (marbleCPool->get(*itBackwards).type != type)
		{
			itBackwards++;
			break;
		}

		count++;

		if (itBackwards == marbles.begin())
			break;

		itBackwards--;
	}

	if (count >= popThreshold)
	{
		if (itForwards != marbles.end())
			marbleCPool->get(*itForwards).separated = true;

		auto transformPool = ComponentPoolManager::getInstance().getPool<Transform>();

		auto itCurrent = itBackwards;
		while (itCurrent != itForwards)
		{
			auto& transform = transformPool->get(*itCurrent);
			auto& marbleInfo = marbleCPool->get(*itCurrent);
			
			Entity particleBurst = EntityManager::getInstance().createEntity();
			particleBurst.addComponent<Transform>(Transform(transform.x, transform.y));
			particleBurst.addComponent<Counter>(Counter(0, 90));
			auto& emitter = particleBurst.addComponent<Emitter>(Emitter(150, 150, 10, 10));

			emitter.emitter.defaultProperties.particleLifetime = { 20, 50 };
			emitter.emitter.defaultProperties.velocityDecay = 0.95;
			emitter.emitter.defaultProperties.yVelVar = { -3, 3 };
			emitter.emitter.defaultProperties.xVelVar = { -3, 3 };
			emitter.emitter.defaultProperties.startSize = 10;
			emitter.emitter.defaultProperties.startColour = { marbleInfo.color.x, marbleInfo.color.y, marbleInfo.color.z, 1 };
			emitter.emitter.defaultProperties.endColour = { marbleInfo.color.x, marbleInfo.color.y, marbleInfo.color.z, 0 };

			SystemsManager::getInstance().getSystem<ParticleS>()->addEntity(particleBurst.getID());
			SystemsManager::getInstance().getSystem<CounterKillerS>()->addEntity(particleBurst.getID());

			EntityManager::getInstance().deleteEntity(*itCurrent);
			itCurrent = marbles.erase(itCurrent);
		}
		return true;
	}

	return false;
}
