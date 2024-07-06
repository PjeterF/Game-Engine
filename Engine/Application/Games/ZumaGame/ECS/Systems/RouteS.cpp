#include "RouteS.hpp"

#include "../../functions.hpp"

RouteS::RouteS(std::string systemID, std::vector<glm::vec2> ctrlPts, unsigned int nSamples, float speed, int nMarbles)
	: ctrlPts(ctrlPts), nSamples(nSamples), speed(speed), nMarbles(nMarbles), systemID(systemID)
{
	requiredComponents = {
		std::type_index(typeid(Transform)),
		std::type_index(typeid(Velocity)),
		std::type_index(typeid(MarbleComponent)),
		std::type_index(typeid(AABB))
	};

	calculateIntermediatePoints();
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

		if (it != marbles.begin())
		{
			itPrev--;
			auto& transform2 = transPool->get(*itPrev);
			auto& velocity2 = velPool->get(*itPrev);
			auto& mComponent2 = mCompPool->get(*itPrev);
			auto& collider2 = colPool->get(*itPrev);

			glm::vec2 diff = { transform.x - transform2.x, transform.y - transform2.y };
			prevDistance = glm::length(diff);
		}

		bool shouldMove;
		if (prevDistance < distanceBetween && nextDistance > distanceBetween*0.9)
			shouldMove = true;
		else
			shouldMove = false;

		if (!shouldMove)
		{
			velocity.x = 0;
			velocity.y = 0;
		}
		else
		{
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
	for (int i = 0; i < intermediatePts.size() - 1; i++)
	{
		rAPI->drawLine(intermediatePts[i], intermediatePts[i + 1], 5, { 0.33, 0.11, 0.11 });
	}

	for (int i = 0; i < ctrlPts.size(); i++)
	{
		glm::vec4 color = {1, 1, 0, 1};
		if (i % 3 == 0)
			color *= 0.88;
		if (i % 3 == 1)
			color *= 0.75;
		if (i % 3 == 2)
			color *= 0.66;
		rAPI->addQuadInstance(ctrlPts[i], { 5, 5 }, 0, color);
	}
	rAPI->drawQuadInstances();
}

void RouteS::calculateIntermediatePoints()
{
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

void RouteS::spawnMarbleAtOrigin(std::string marbleArchetypeFilepath, MovementS& msys, RenderingS& rsys, CollisionS& csys, AnimationS& asys)
{
	if (ctrlPts.empty())
		return;

	int marbleID = ZumaFn::spawnMarble(marbleArchetypeFilepath, ctrlPts[0], systemID);
	if (marbleID < 0)
		return;

	Entity marble(marbleID);

	if (!intermediatePts.empty())
		marble.getComponent<MarbleComponent>().targetPointIdx = 0;

	msys.addEntity(marbleID);
	rsys.addEntity(marbleID);
	csys.addEntity(marbleID);
	asys.addEntity(marbleID);
	this->addEntity(marbleID);

	marbles.push_front(marbleID);
}

void RouteS::spawnMarbleIfPossible(std::string marbleArchetypeFilepath, MovementS& msys, RenderingS& rsys, CollisionS& csys, AnimationS& asys)
{
	if (ctrlPts.empty() || nMarbles<=0)
		return;

	if (marbles.empty())
	{
		nMarbles--;
		spawnMarbleAtOrigin(marbleArchetypeFilepath, msys, rsys, csys, asys);
		return;
	}

	Entity firstMarble(marbles.front());
	auto& transform = firstMarble.getComponent<Transform>();
		
	if (glm::length(glm::vec2(transform.x - ctrlPts[0].x, transform.y - ctrlPts[0].y))>distanceBetween*0.9)
	{
		nMarbles--;
		spawnMarbleAtOrigin(marbleArchetypeFilepath, msys, rsys, csys, asys);
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
					//it++;

				if (it != marbles.end())
				{
					auto& itTrans = transformPool->get(*it);

					inserteeTransform.x = itTrans.x;
					inserteeTransform.y = itTrans.y;
				}

				marbles.insert(it, inserteeID);

				if (popSame(it))
					return true;


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
		do
		{
			EntityManager::getInstance().deleteEntity(*itBackwards);
		} while (++itBackwards != itForwards);

		if (itForwards != marbles.end())
			EntityManager::getInstance().deleteEntity(*itForwards);

		return true;
	}

	return false;
}
