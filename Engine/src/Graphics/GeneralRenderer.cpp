#include "GeneralRenderer.hpp"

RenderingAPI::RenderingAPI(Camera* camera, GLuint lineShaderID, GLuint spriteShaderID, GLuint quadShaderID)
{
	lineRenderer = new LineRenderer(lineShaderID, camera);
	spriteRenderer = new SpriteRenderer(spriteShaderID, camera);
	quadRenderer = new QuadRenderer(quadShaderID, camera);
}

Camera* RenderingAPI::getCamera()
{
	return camera;
}

void RenderingAPI::setCamera(Camera* camera)
{
	this->camera = camera;
	lineRenderer->camera = camera;
	spriteRenderer->camera = camera;
	quadRenderer->camera = camera;
	instancedQuadRenderer->camera = camera;
}

void RenderingAPI::drawSprite(glm::vec2 position, glm::vec2 scale, float angle, Texture* texture)
{
	spriteRenderer->draw(position, scale, angle, texture);
}

void RenderingAPI::drawSpriteSampled(glm::vec2 position, glm::vec2 scale, float angle, Texture* texture, TextureDivision division)
{
	spriteRenderer->draw(position, scale, angle, texture, division);
}

void RenderingAPI::drawQuad(glm::vec2 position, glm::vec2 scale, float angle, glm::vec4 colour)
{
	quadRenderer->draw(position, scale, angle, colour);
}

void RenderingAPI::drawLine(glm::vec2 p1, glm::vec2 p2, float width, glm::vec3 colour)
{
	lineRenderer->draw(p1.x, p1.y, p2.x, p2.y, width, colour.x, colour.y, colour.z);
}

void RenderingAPI::addQuadInstance(glm::vec2 position, glm::vec2 scale, float angle, glm::vec4 colour)
{
	instancedQuadRenderer->commisionInstance(position.x, position.y, scale.x, angle, colour.x, colour.y, colour.z, colour.w);
}

void RenderingAPI::drawQuadInstances()
{
	instancedQuadRenderer->drawInstances();
}
