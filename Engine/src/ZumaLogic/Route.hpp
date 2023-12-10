#pragma once

#include "CubicBezierSpline.hpp"
#include "Segment.hpp"
#include "Snake.hpp"

#include <nlohmann/json.hpp>

class Route : public Entity, public EventObserver
{
public:
	Route(CubicBezierSpline* spline);
	~Route();
	virtual void handleEvent(Event& event);
	void setInitialSnake(int numSegments, int maxConcurrentSameTypeSegments, float velocity, float segmentSize);
	virtual void update();
	virtual void draw(RenderingAPI* renderer);
	virtual nlohmann::json serialize();
	static Route* deSerialize(nlohmann::json j);
	virtual bool setLayer(int targetLayer);

	Snake* initialSnake = nullptr;
	CubicBezierSpline* spline;
private:
	int numSegments, maxConcurrentSameTypeSegments;
	float velocity, segmentSize;
};