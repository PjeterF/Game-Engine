#include "CubicBezierSpline.hpp"
#include "../Managers/Resource/ResourceManager.hpp"

void CubicBezierSpline::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::CtrlPointPlacement:
	{
		glm::vec2* mousePos = (glm::vec2*)event.getPayload();
		this->addSegment(mousePos->x, mousePos->y);
	}
	break;
	case Event::CtrlPointMovement:
	{
		glm::vec2* mousePos = (glm::vec2*)event.getPayload();
		if (!moving)
		{
			float delta = 20;
			for (int i = 0; i<controlPoints.size(); i++)
			{
				if (mousePos->x > controlPoints.at(i).x - delta && mousePos->x < controlPoints.at(i).x + delta &&
					mousePos->y > controlPoints.at(i).y - delta && mousePos->y < controlPoints.at(i).y + delta)
				{
					moving = true;
					movingIndex = i;
				}
			}
		}
		else
		{
			controlPoints.at(movingIndex).x = mousePos->x;
			controlPoints.at(movingIndex).y = mousePos->y;
			sample();
			moving = false;
		}
	}
	break;
	case Event::MouseMove:
	{
		if (moving)
		{
			glm::vec2* mousePos = (glm::vec2*)event.getPayload();
			controlPoints.at(movingIndex).x = mousePos->x;
			controlPoints.at(movingIndex).y = mousePos->y;
			sample();
		}
	}
	break;
	case Event::TogglePause:
	{
		hidden = !hidden;
	}
	break;
	}
}

nlohmann::json CubicBezierSpline::serialize()
{
	nlohmann::json j;
	
	for (auto point : controlPoints)
	{
		j["controlPoints"].push_back({ point.x, point.y });
	}

	j["samples_per_segment"] = samples_per_segment;
	j["tex1"] = tex1->getContents()->getFilepath();
	j["tex2"] = tex2->getContents()->getFilepath();
	j["control_point_size"] = control_point_size;

	return j;
}

CubicBezierSpline* CubicBezierSpline::deSerialize(nlohmann::json j)
{
	std::vector<glm::vec2> points;
	for (auto it = j["controlPoints"].begin(); it != j["controlPoints"].end(); it++)
	{
		points.push_back(glm::vec2((*it)[0], (*it)[1]));
	}

	if (ResourceManager::getInstance().getResource<Texture>(j["tex1"]) == nullptr)
	{
		ResourceManager::getInstance().createResourceFromFile<Texture>(j["tex1"]);
	}

	if (ResourceManager::getInstance().getResource<Texture>(j["tex2"]) == nullptr)
	{
		ResourceManager::getInstance().createResourceFromFile<Texture>(j["tex2"]);
	}


	return new CubicBezierSpline
	(
		points,
		j["samples_per_segment"],
		ResourceManager::getInstance().getResource<Texture>(j["tex1"]),
		ResourceManager::getInstance().getResource<Texture>(j["tex2"]),
		j["control_point_size"]
	);
}

int CubicBezierSpline::getNumberOfSegments()
{
	return (int)((controlPoints.size() - 1) / 3);
}

std::vector<glm::vec2>* CubicBezierSpline::getSampledPoints()
{
	return &sampledPoints;
}

glm::vec2 CubicBezierSpline::getValueAt(float t)
{

	if (controlPoints.size() < 4)
		return glm::vec2(0, 0);

	int splineIndex = trunc(t);

	int n = getNumberOfSegments();
	if (splineIndex < n)
	{
		t = t - trunc(t);
	}
	else
	{
		splineIndex--;
		t = 1;
	}

	glm::vec2 t0 = (1 - t) * (1 - t) * (1 - t) * controlPoints[3 * splineIndex + 0];
	glm::vec2 t1 = 3 * (1 - t) * (1 - t) *t* controlPoints[3 * splineIndex + 1];
	glm::vec2 t2 = 3 * (1 - t) *t*t* controlPoints[3 * splineIndex + 2];
	glm::vec2 t3 = t*t*t* controlPoints[3 * splineIndex + 3];
	return t0 + t1 + t2 + t3;
}

void CubicBezierSpline::sample()
{
	std::vector <glm::vec2> samples;
	float t;
	for (int i = 0; i < getNumberOfSegments(); i++)
	{
		for (int j = 0 ; j < samples_per_segment + 1; j++)
		{
			t = i + j * (1 / (samples_per_segment + static_cast<float>(1)));
			samples.push_back(getValueAt(t));
		}
	}
	samples.push_back(getValueAt(getNumberOfSegments()));

	sampledPoints = samples;
}

CubicBezierSpline::CubicBezierSpline(float xInit, float yInit, int samples_per_segment, Resource<Texture>* texture1, Resource<Texture>* texture2, float control_point_size)
{
	type = spline;
	this->samples_per_segment = samples_per_segment;
	this->tex1 = texture1;
	texture1->subscribe();
	this->tex2 = texture2;
	texture2->subscribe();
	this->control_point_size = control_point_size;

	controlPoints.push_back(glm::vec2(xInit,yInit));

	sample();
}

CubicBezierSpline::CubicBezierSpline(std::vector<glm::vec2> controlPoints, int samples_per_segment, Resource<Texture>* texture1, Resource<Texture>* texture2, float control_point_size)
{
	type = spline;
	this->samples_per_segment = samples_per_segment;
	this->tex1 = texture1;
	texture1->subscribe();
	this->tex2 = texture2;
	texture2->subscribe();	
	this->control_point_size = control_point_size;
	this->controlPoints = controlPoints;

	sample();
}

CubicBezierSpline::~CubicBezierSpline()
{
	tex1->unsubscribe();
	tex2->unsubscribe();
}

std::vector<glm::vec2>* CubicBezierSpline::getControlPoints()
{
	return &controlPoints;
}

void CubicBezierSpline::drawLines(LineRenderer* renderer)
{
	if (sampledPoints.size() <3)
		return;

	for (int i = 0; i < controlPoints.size()-1; i=i+3)
	{
		renderer->draw(controlPoints[i].x, controlPoints[i].y, controlPoints[i + 1].x, controlPoints[i + 1].y, width, 1, 0, 0);
		renderer->draw(controlPoints[i+2].x, controlPoints[i+2].y, controlPoints[i + 3].x, controlPoints[i + 3].y, width, 1, 0, 0);
	}

	for (int i = 0; i < sampledPoints.size() - 1; i++)
	{
		renderer->draw(sampledPoints[i].x, sampledPoints[i].y, sampledPoints[i + 1].x, sampledPoints[i + 1].y, width, 1.0, 0.5, 0);
	}
}

void CubicBezierSpline::addSegment(float x, float y)
{
	glm::vec2 last = controlPoints.back();
	glm::vec2 direction = glm::vec2(x, y) - last;
	controlPoints.push_back(glm::vec2(last.x + 0.33f * direction.x, last.y + 0.33f * direction.y));
	controlPoints.push_back(glm::vec2(last.x + 0.66f * direction.x, last.y + 0.66f * direction.y));
	controlPoints.push_back(glm::vec2(x, y));

	if (this->controlPoints.size() > 3)
		this->sample();
}

void CubicBezierSpline::drawControlPoints(SpriteRenderer* renderer)
{
	for (int i = 0; i < controlPoints.size(); i++)
	{
		if (i % 3 == 0)
		{
			renderer->draw(controlPoints[i], glm::vec2(control_point_size, control_point_size), 0, tex1->getContents());
		}
		else
		{
			renderer->draw(controlPoints[i], glm::vec2(control_point_size, control_point_size), 0, tex2->getContents());
		}
	}
}

void CubicBezierSpline::draw(RenderingAPI* renderer)
{
	if (!hidden)
	{
		this->drawLines(renderer->lineRenderer);
		this->drawControlPoints(renderer->spriteRenderer);
	}
}

void CubicBezierSpline::update()
{
}

bool CubicBezierSpline::removeLastSegment()
{
	if (controlPoints.size() > 3)
	{
		controlPoints.pop_back();
		controlPoints.pop_back();
		controlPoints.pop_back();
		this->sample();
		return true;
	}
	else
	{
		return false;
	}
}