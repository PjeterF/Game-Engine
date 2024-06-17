#pragma once

#include "SysBase.hpp"

#include "../../Graphics/GeneralRenderer.hpp"

#include "../Components/Transform.hpp"
#include "../Components/Sprite.hpp"
#include "../Components/Animation.hpp"

class RenderingS : public SysBase
{
public:
	RenderingS(RenderingAPI* rAPI);
	void update(float dt) override;
	virtual bool addEntity(int ID, int layer=0);
	virtual bool addEntity(Entity ent, int layer=0);
	virtual void removeEntity(int ID) override;
private:
	RenderingAPI* rAPI;
	std::set<int> animatedEntities;
	std::map<int, std::set<int>> layers;
};