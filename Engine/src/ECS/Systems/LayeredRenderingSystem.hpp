#pragma once

#include "SystemBase.hpp"
#include "../../Graphics/GeneralRenderer.hpp"

class LayeredRenderingSystem : public SystemBase
{
public:
	static LayeredRenderingSystem& getInstance();
	static void initialize(RenderingAPI* API);

	virtual void update(float dt) override;
	virtual bool addEntity(Ent* entity) override;
	virtual void removeEntity(int ID) override;

	virtual void handleEvent(Event& event) override;
private:
	static LayeredRenderingSystem& instanceInplementation(RenderingAPI* API = nullptr);
	LayeredRenderingSystem(RenderingAPI* API);

	RenderingAPI* API;
	std::list<Ent*> orderedEntities;
};