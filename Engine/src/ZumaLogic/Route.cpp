#include "Route.hpp"

Route::Route(CubicBezierSpline* spline)
{
	type = route;
	this->spline = spline;
}

Route::~Route()
{
	if(spline!=nullptr)
		spline->destroy();
	if (initialSnake != nullptr)
		initialSnake->destroy();
}

void Route::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::Reinitialize:
	{
		setInitialSnake(numSegments, maxConcurrentSameTypeSegments, velocity, segmentSize);
	}
	break;
	default:
		break;
	}
}

void Route::setInitialSnake(int numSegments, int maxConcurrentSameTypeSegments, float velocity, float segmentSize)
{
	if (initialSnake != nullptr)
	{
		initialSnake->destroy();
		EntityManager::getInstance().update();
		EntityManager::getInstance().update();
	}

	initialSnake = new Snake(numSegments, segmentSize, maxConcurrentSameTypeSegments, velocity, spline);
	initialSnake->setLayer(this->layer);
	this->numSegments = numSegments;
	this->segmentSize = segmentSize;
	this->maxConcurrentSameTypeSegments = maxConcurrentSameTypeSegments;
	this->velocity = velocity;
}

void Route::update()
{
}

void Route::draw(RenderingAPI* renderer)
{
}

nlohmann::json Route::serialize()
{
	nlohmann::json j = this->spline->serialize();
	
	j["numSegments"] = numSegments;
	j["segmentSize"] = segmentSize;
	j["maxConcurrentSameTypeSegments"] = maxConcurrentSameTypeSegments;
	j["velocity"] = velocity;

	return j;
}

Route* Route::deSerialize(nlohmann::json j)
{
	CubicBezierSpline* spline = CubicBezierSpline::deSerialize(j);

	Route* route = new Route(spline);
	route->setInitialSnake(j["numSegments"], j["maxConcurrentSameTypeSegments"], j["velocity"], j["segmentSize"]);

	return route;
}

bool Route::setLayer(int targetLayer)
{
	if (Entity::setLayer(targetLayer))
	{
		if(initialSnake!=nullptr)
			initialSnake->setLayer(targetLayer);
		spline->setLayer(targetLayer);
	}
	else
	{
		return false;
	}
}
