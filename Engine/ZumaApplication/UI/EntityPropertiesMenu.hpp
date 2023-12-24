#pragma once

#include "../../src/ECS/Components/TransformC.hpp"
#include "../../src/ECS/Components/SpriteC.hpp"
#include "../../src/ECS/Components/VelocityC.hpp"
#include "../../src/ECS/Components/BoxColliderC.hpp"
#include "../../src/ECS/Components/AnimatedSpriteC.hpp"
#include "../../src/ECS/Components/RenderingLayerC.hpp"
#include "../../src/ECS/Components/ParticleC.hpp"
#include "../../src/ECS/Components/CounterC.hpp"

#include "../Logic/ShooterInfoC.hpp"

#include "InterfaceWindow.hpp"
#include "../../src/Events/Event.hpp"
#include "../../src/Events/EventObserver.hpp"

class EntityPropertiesMenu : public InterfaceWindow, public EventObserver
{
public:
	EntityPropertiesMenu(int x, int y, int width, int height);
	void draw();
	void selectEntity(Ent* entity);
	virtual void handleEvent(Event& event) override;
private:
	void componentAdditionMenu();
	void addToSystemMenu();

	void transformProp(TransformC* component);
	void spriteProp(SpriteC* component);
	void velocityProp(VelocityC* component);
	void colliderProp(BoxColliderC* component);
	void animatedSpriteProp(AnimatedSpriteC* component);
	void renderingLayerProp(RenderingLayerC* component);
	void shooterProp(ShooterC* component);
	void particleProp(ParticleC* component);
	void counterProp(CounterC* component);

	Ent* selectedEntity = nullptr;
	bool compRemoved = false;
};