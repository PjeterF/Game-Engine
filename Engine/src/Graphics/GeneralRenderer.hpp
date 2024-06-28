#pragma once

#include "LineRenderer.hpp"
#include "SpriteRenderer.hpp"
#include "TextRenderer.hpp"
#include "QuadRenderer.hpp"
#include "InstancedQuadRenderer.hpp"
#include "InstancedSpriteRenderer.hpp"
#include "Camera.hpp"

class RenderingAPI
{
public:
	RenderingAPI(Camera* camera, GLuint lineShaderID, GLuint spriteShaderID, GLuint quadShaderID, GLuint instancedQuadShaderId, GLuint instancedSpriteShaderId);
	LineRenderer* lineRenderer = nullptr;
	SpriteRenderer* spriteRenderer = nullptr;
	QuadRenderer* quadRenderer = nullptr;
	InstancedQuadRenderer* instancedQuadRenderer = nullptr;
	InstancedSpriteRenderer* instancedSpriteRenderer = nullptr;

	Camera* getCamera();
	void setCamera(Camera* camera);

	void drawSprite(glm::vec2 position, glm::vec2 scale, float angle, Texture* texture);
	void drawSpriteSampled(glm::vec2 position, glm::vec2 scale, float angle, Texture* texture, TextureDivision division);
	void drawQuad(glm::vec2 position, glm::vec2 scale, float angle, glm::vec4 colour);
	void drawLine(glm::vec2 p1, glm::vec2 p2, float width, glm::vec3 colour);
	void addQuadInstance(glm::vec2 position, glm::vec2 scale, float angle, glm::vec4 colour);
	void drawQuadInstances();
	void addSpriteInstance(glm::vec2 position, glm::vec2 scale, float angle, Texture* texture, glm::vec4 texTransform = {0, 0, 0, 0}, bool flipHorizontally=false);
	void drawSpriteInstances();
private:
	Camera* camera;
};