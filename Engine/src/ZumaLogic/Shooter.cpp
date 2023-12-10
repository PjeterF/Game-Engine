#include "Shooter.hpp"
#include "Segment.hpp"
#include "../Managers/TexturesManager.hpp"

Shooter::Shooter(int x, int y, int size, Resource<Texture>* texture, float shotVelocity, float shotSize) : Sprite(x, y, size, texture)
{
	type = shooter;
	this->collision = false;
	this->shotVelocity = shotVelocity;
	this->shotSize = shotSize;

	if (Segment::segmentTemplates.size() == 0)
	{
		currentProjectileType = -1;
		nextProjectileType = -1;
	}
	else
	{
		currentProjectileType = rand() % Segment::segmentTemplates.size();
		nextProjectileType = rand() % Segment::segmentTemplates.size();
	}

	EventManager::getInstance().notify(Event(Event::ShooterCreation, this));
}

void Shooter::lookAt(float x, float y)
{
	glm::vec2 vector = glm::vec2(x, y) - position;
	vector = glm::normalize(vector);

	if (x >= position.x && y >= position.y)
	{
		angle = atan(vector.y / vector.x) * 57.2958 + 90;
	}
	else if (x < position.x && y >= position.y)
	{
		angle = 180 - atan(-vector.y / vector.x) * 57.2958 + 90;
	}
	else if (x < position.x && y < position.y)
	{
		angle = 180 + atan(vector.y / vector.x) * 57.2958 + 90;
	}
	else if (x >= position.x && y < position.y)
	{
		angle = 360 - atan(-vector.y / vector.x) * 57.2958 + 90;
	}
}

void Shooter::shoot(float x, float y)
{
	if(Segment::segmentTemplates.size() == 0)
		return;

	glm::vec2 direction = glm::normalize(glm::vec2(x, y) - position);
	Segment* newSegment = new Segment(position.x, position.y, Segment::segmentTemplates[currentProjectileType]);
	newSegment->direction = direction;
	newSegment->velocity = shotVelocity;
	newSegment->visible = true;
	newSegment->collision = true;
	newSegment->setLayer(this->layer);
	projectiles.push_back(newSegment);

	currentProjectileType = nextProjectileType;
	if (Segment::segmentTemplates.size()==0)
		nextProjectileType = -1;
	else
		nextProjectileType = rand() % Segment::segmentTemplates.size();
}

void Shooter::reroll()
{
	if (Segment::segmentTemplates.size() == 0)
	{
		currentProjectileType = 0;
		nextProjectileType = 0;
	}
	else
	{
		currentProjectileType = rand() % Segment::segmentTemplates.size();
		nextProjectileType = rand() % Segment::segmentTemplates.size();
	}

}

void Shooter::draw(RenderingAPI* renderer)
{
	renderer->spriteRenderer->draw(position, size, angle, texture->getContents());

	if(Segment::segmentTemplates.size()==0)
		return;

	if (Segment::segmentTemplates[currentProjectileType].frames.size() > 0)
	{
		renderer->spriteRenderer->draw(position, glm::vec2(shotSize, shotSize), 0, Segment::segmentTemplates[currentProjectileType].texture->getContents(), Segment::segmentTemplates[currentProjectileType].frames[0]);
	}
	else
	{
		renderer->spriteRenderer->draw(position, glm::vec2(shotSize, shotSize), 0, Segment::segmentTemplates[currentProjectileType].texture->getContents());
	}
}

void Shooter::update()
{
	updateCollider(position.x, position.y, size.x, size.y);
}

void Shooter::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::MouseMove:
	{
		glm::vec2* pos = (glm::vec2*)event.getPayload();
		lookAt(pos->x, pos->y);
	}
	break;
	case Event::Shoot:
	{
		glm::vec2* pos = (glm::vec2*)event.getPayload();
		if(shootingEnabled)
			shoot(pos->x, pos->y);
	}
	break;
	case Event::TogglePause:
	{
		shootingEnabled = !shootingEnabled;
	}
	break;
	case Event::Reinitialize:
	{
		reroll();
	}
	break;
	default:
		break;
	}
}

nlohmann::json Shooter::serialize()
{
	nlohmann::json j=Sprite::serialize();

	j["shotVelocity"] = shotVelocity;
	j["shotSize"] = shotSize;

	return j;
}

Shooter* Shooter::deSerialize(nlohmann::json j)
{
	float shotSize = j["shotSize"];
	float shotVelocity = j["shotVelocity"];
	float angle = j["angle"];
	bool collision = j["collision"];
	glm::vec2 position = glm::vec2(j["position"][0], j["position"][1]);
	float size = j["size"][0];
	float velocity = j["velocity"];

	if (ResourceManager::getInstance().getResource<Texture>(j["texture"]) == nullptr)
		ResourceManager::getInstance().createResourceFromFile<Texture>(j["texture"]);

	return new Shooter(position.x, position.y, size, ResourceManager::getInstance().getResource<Texture>(j["texture"]), shotVelocity, shotSize);
}
