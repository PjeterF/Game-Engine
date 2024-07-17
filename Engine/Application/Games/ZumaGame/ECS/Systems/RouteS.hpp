#pragma once

#include "../../src/ECS2/Systems/SysBase.hpp"
#include "../../src/ECS2/Systems/MovementS.hpp"
#include "../../src/ECS2/Systems/RenderingS.hpp"
#include "../../src/ECS2/Systems/CollisionS.hpp"
#include "../../src/ECS2/Systems/AnimationS.hpp"
#include "../../src/Graphics/GeneralRenderer.hpp"

#include "../Components/MarbleComponent.hpp"
#include"../../src/ECS2/Components/Transform.hpp"
#include"../../src/ECS2/Components/Velocity.hpp"

#include <vector>
#include <list>

class RouteS : public SysBase
{
public:
	RouteS(std::string systemID, std::vector<glm::vec2> ctrlPts = { {0, 0} }, unsigned int nSamples = 10, float speed = 2, int nMarbles = 100, bool deleteOnSceneEnd = true);
	virtual void handleEvent(Event& event) override;
	virtual void update(float dt) override;
	void draw(RenderingAPI* rAPI);
	void calculateIntermediatePoints();
	void setNSamples(unsigned int nSamples);
	void addSegment(glm::vec2 endPoint);
	void removeLastSegment();
	void spawnMarbleAtOrigin(std::string marbleArchetypeFilepath);
	void spawnMarbleIfPossible(std::string marbleArchetypeFilepath);
	void setControlPointPos(int index, glm::vec2 newPos);
	const std::string& getID();
	bool insertAt(int entID, int inserteeID);
	void reset();
	void setNumberOfMarbles(int n);

	int ctrlPointIntersection(glm::vec2 pos);

	float speed;
	int popThreshold = 3;
	float distanceBetween = 40;
private:
	bool popSame(std::list<int>::iterator it);

	unsigned int nSamples;
	unsigned int nMarbles;
	unsigned int remainingMarbles;
	
	std::string systemID;

	std::list<int> marbles;

	std::vector<glm::vec2> ctrlPts;
	std::vector<glm::vec2> intermediatePts;
};