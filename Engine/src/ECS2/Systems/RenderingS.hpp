#pragma once

#include "SysBase.hpp"

#include "../../Graphics/GeneralRenderer.hpp"

#include "../Components/Transform.hpp"
#include "../Components/Sprite.hpp"
#include "../Components/Emitter.hpp"
#include "../Components/Animation.hpp"
#include "../Components/Velocity.hpp"
#include "../Components/RenderingLayer.hpp"

#include <vector>

class RenderingS : public SysBase
{
public:
	RenderingS(RenderingAPI* rAPI);
	void update(float dt) override;
	virtual bool addEntity(int ID);
	virtual bool addEntity(Entity ent);
	virtual void removeEntity(int ID) override;

	void ySortLayer(int layer);
	bool moveToLayer(int ID, int newLayer);
private:
	RenderingAPI* rAPI;
	std::map<int, std::vector<int>> layers;
};