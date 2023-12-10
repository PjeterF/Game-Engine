#pragma once

#include "Segment.hpp"
#include "CubicBezierSpline.hpp"
#include "../Events/EventPackage.hpp"
#include "../Managers/EntityManager.hpp"

#include <vector>
#include <time.h>

class Snake : public Entity, public EventObserver
{
public:
	Snake(int initalSegmentNumber, float segmentSize, int maxSameSegments, float velocity, CubicBezierSpline* spline);
	virtual ~Snake();
	virtual void handleEvent(Event& event);

	virtual void draw(RenderingAPI* renderer) override;
	virtual void update() override;
	Snake* splice(std::list<Segment*>::iterator segment);
	void updateOwnership();
	nlohmann::json serialize();
	static Snake* deSerialize(nlohmann::json j, CubicBezierSpline* spline);
	void updateSegmentsLayer();
	virtual bool setLayer(int targetLayer);

	bool checkPoppingCondition(std::list<Segment*>::iterator segmente);
	void popSame(std::list<Segment*>::iterator segment);

	std::list<Segment*> segments;
	CubicBezierSpline* spline;
	float velocity;
	bool moving = true;
	bool inserting = false;
	int initialSegmentNumber;
	float segmentSize;
	int maxSameSegments;
	int segmentsToSpawn=0;
private:
	int popingCount = 3;
	int insertingDuration = Segment::insetringDuration;

	Snake() {};
	void makeVisible(Segment* segment);
	void spawnSegment();
	int counter = 0;
};