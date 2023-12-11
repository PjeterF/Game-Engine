#pragma once
#include <list>
#include <glm/vec2.hpp>
#include <cmath>
#include <vector>
#include "../../src/Graphics/GeneralRenderer.hpp"
#include "../../src/Managers/Resource/ResourceManager.hpp"
#include "../../src/Events/EventManager.hpp"

class Spline : public EventObserver
{
public:
	Spline(float xInit, float yInit, int samples_per_segment, Resource<Texture>* texture1, Resource<Texture>* texture2, float control_point_size);
	Spline(std::vector<glm::vec2> controlPoints, int samples_per_segment, Resource<Texture>* texture1, Resource<Texture>* texture2, float control_point_size);
	~Spline();

	virtual void handleEvent(Event& event) override;

	int getNumberOfSegments();
	glm::vec2 getValueAt(float t);
	std::vector<glm::vec2>* getSampledPoints();
	std::vector<glm::vec2>* getControlPoints();
	void sample();

	void draw(RenderingAPI* renderer);

	void addSegment(float x, float y);
	bool removeLastSegment();

	int samples_per_segment;
	float control_point_size;
	int width = 1;
	bool hidden = false;

	std::vector<glm::vec2> controlPoints;
	std::vector<glm::vec2> sampledPoints;
	Resource<Texture>* tex1;
	Resource<Texture>* tex2;
private:
	void drawLines(LineRenderer* renderer);
	void drawControlPoints(SpriteRenderer* renderer);

	bool moving = false;
	int movingIndex = -1;
};