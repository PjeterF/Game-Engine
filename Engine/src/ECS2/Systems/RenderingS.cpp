#include "RenderingS.hpp"

RenderingS::RenderingS(RenderingAPI* rAPI) : rAPI(rAPI)
{
	requiredComponents = { std::type_index(typeid(Transform)), std::type_index(typeid(Sprite)) };
}

void RenderingS::update(float dt)
{
	for (auto entID : entities)
	{
		Entity ent(entID);

		Transform& trans = ent.getComponent<Transform>();
		Sprite& sprite = ent.getComponent<Sprite>();

		rAPI->addSpriteInstance({ trans.x, trans.y }, { trans.width, trans.height }, trans.rot, sprite.getTexture()->getContents());
	}
	rAPI->drawQuadInstances();
}
