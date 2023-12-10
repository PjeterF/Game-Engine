#pragma once
#include <list>
#include <glm/vec2.hpp>
#include <cmath>
#include <vector>
#include "../Graphics/SpriteRenderer.hpp"
#include "../Graphics/GeneralRenderer.hpp"
#include "../Graphics/LineRenderer.hpp"
#include "../Events/EventPackage.hpp"
#include "../Managers/Entity.hpp"
#include"../Managers/Resource/ResourceManager.hpp"

class CubicBezierSpline : public EventObserver, public Entity
{
public:
	CubicBezierSpline(float xInit, float yInit, int samples_per_segment, Resource<Texture>* texture1, Resource<Texture>* texture2, float control_point_size);
	CubicBezierSpline(std::vector<glm::vec2> controlPoints, int samples_per_segment, Resource<Texture>* texture1, Resource<Texture>* texture2, float control_point_size);
	~CubicBezierSpline();
	int getNumberOfSegments();
	glm::vec2 getValueAt(float t);
	std::vector<glm::vec2>* getSampledPoints();
	std::vector<glm::vec2>* getControlPoints();
	void sample();

	virtual void draw(RenderingAPI* renderer) override;
	virtual void update();
	void addSegment(float x, float y);
	bool removeLastSegment();

	virtual void handleEvent(Event& event) override;
	nlohmann::json serialize();
	static CubicBezierSpline* deSerialize(nlohmann::json j);

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