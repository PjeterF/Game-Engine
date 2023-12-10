#pragma once
#include "../OpenGL/texture.hpp"
#include "../Graphics/spriterenderer.hpp"
#include "../Graphics/GeneralRenderer.hpp"
#include "../Managers/Collision/Collider.hpp"
#include "glm/vec2.hpp"
#include <math.h>
#include "../Managers/Entity.hpp"
#include "../Managers/Resource/ResourceManager.hpp"
#include <nlohmann/json.hpp>

class Sprite : public Entity, public Collider
{
public:
	Sprite(float x, float y, float size, Resource<Texture>* texture, bool collision = true, float angle = 0, int layer = 0);
	Sprite(float x, float y, float sizeX, float sizeY, Resource<Texture>* texture, bool collision=true, float angle=0, int layer = 0);
	virtual ~Sprite();
	virtual void draw(RenderingAPI* renderer) override;
	virtual void update() override;
	virtual nlohmann::json serialize();
	static Sprite* deSerialize(nlohmann::json j);

	glm::vec2 position;
	glm::vec2 size;
	float angle = 0;
	float velocity = 0;
	glm::vec2 direction = glm::vec2(0, 0);
	Resource<Texture>* texture;
};