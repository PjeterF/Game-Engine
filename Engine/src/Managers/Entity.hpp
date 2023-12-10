#pragma once

#include "../Graphics/GeneralRenderer.hpp"
#include <nlohmann/json.hpp>

class Entity
{
public:
	Entity(int layer=0);
	virtual ~Entity();
	enum EntityType
	{
		null,
		generic,
		shot,
		snake,
		shooter,
		spline,
		segment,
		route,
		sprite,
		despawner,
		emitter,
		camera
	};

	virtual void draw(RenderingAPI* renderer)=0;
	virtual void update()=0;
	void destroy();
	virtual bool setLayer(int targetLayer);

	int getID();

	EntityType type=null;
	int layer = 0;
	bool pendingDelete = false;
private:
	int generateID();
	int ID;
};