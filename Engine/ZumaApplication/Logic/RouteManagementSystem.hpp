#pragma once

#include "../../src/ECS/Systems/SystemBase.hpp"
#include "../../src/ECS/Systems/CollisionSystem.hpp"
#include "../../src/ECS/Components/BoxColliderC.hpp"
#include "../../src/ECS/Components/TransformC.hpp"
#include "../../src/ECS/Components/VelocityC.hpp"
#include "../../src/ECS/Components/BoxColliderC.hpp"
#include "../../src/ECS/Components/AnimatedSpriteC.hpp"
#include "../../src/ECS/Components/RenderingLayerC.hpp"
#include "MarbleCollisionResolutionSystem.hpp"

#include "Spline.hpp"

#include <list>

struct MarbleTemplate
{
public:
	MarbleTemplate(float size, int tag, std::string textureFilepath, std::vector<TextureDivision> divisions, int frameDuration, glm::vec3 explosionColor);
	float size;
	int tag;
	std::string textureFilepath;
	std::vector<TextureDivision> divisions;
	int frameDuration;
	glm::vec3 explosionColor;
};

class RouteManagementSystem : public SystemBase
{
public:
	RouteManagementSystem(Spline* spline, int initialMarbleNumber = 100);
	virtual ~RouteManagementSystem();

	virtual void to_json(nlohmann::json& j) const override;
	virtual void from_json(nlohmann::json& j) override;

	virtual void update(float dt) override;
	virtual void handleEvent(Event& event) override;

	void spawnRandomMarble();
	void spawnExplosion(float x, float y, glm::vec3 startColor, glm::vec3 endColor, float startSize, float endSize, float startAlpha, float endAlpha, float vel, int count);
	void setLayer(int target);
	void setInitialMarbleCount(int n);
	int getInitialMarbleCount();
	Spline* getSpline();

	float marbleSpeed = 2;
	int popThreshold = 3;

	void drawSpline(RenderingAPI* API);
	void addRoutePoint(glm::vec2 pos);
	void removeLastRoutePoint();

	static std::vector<MarbleTemplate> marbleTemplates;
private:
	void moveRoutine(std::vector<Ent*>::iterator s_it, float dt);
	int popSame(std::vector<Ent*>::iterator it, int threshold);

	int layer = 0;
	int marblesLeftToSpawn;
	int maxMarbles;

	bool selfDestruct = false;

	std::list<Ent*> marbles;
	Spline* spline;
};