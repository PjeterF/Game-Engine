#include "SpriteRenderer.hpp"

SpriteRenderer::SpriteRenderer(GLuint shaderProgramID, Camera* camera)
{
	this->shaderProgramID = shaderProgramID;
	this->camera = camera;

	std::vector<float> vertices;
	vertices.push_back(-1);
	vertices.push_back(1);
	vertices.push_back(0);
	vertices.push_back(1);

	vertices.push_back(1);
	vertices.push_back(1);
	vertices.push_back(1);
	vertices.push_back(1);

	vertices.push_back(-1);
	vertices.push_back(-1);
	vertices.push_back(0);
	vertices.push_back(0);

	vertices.push_back(1);
	vertices.push_back(-1);
	vertices.push_back(1);
	vertices.push_back(0);

	std::vector<int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	VAO = new VertexArray;
	VAO->bind();

	VBO = new VertexBuffer(vertices);
	VBO->bind();

	EBO = new ElementBuffer(indices);
	EBO->bind();

	VAO->setAttributePointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	VAO->setAttributePointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, sizeof(float) * 2);

	VAO->unbind();
}

void SpriteRenderer::draw(glm::vec2 position, glm::vec2 scale, float angle, Texture* texture)
{
	if (texture == nullptr)
		return;

	if (!camera->isPointInFrustrum(position, 50))
		return;

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::ortho(camera->getOriginalFrustrumX().x, camera->getOriginalFrustrumX().y, camera->getOriginalFrustrumY().x, camera->getOriginalFrustrumY().y, -1.0f, 1.0f);
	transform = glm::translate(transform, glm::vec3(camera->getZoom() * position.x - camera->getPosition().x, camera->getZoom() * position.y - camera->getPosition().y, 0));
	transform = glm::rotate(transform, glm::radians(angle+0), glm::vec3(0, 0, 1));
	transform = glm::scale(transform, glm::vec3(camera->getZoom() * scale.x, camera->getZoom() *scale.y, 1));

	float xOffset = 0;
	float yOffset = 0;
	float xScale = 1;
	float yScale = 1;

	glm::mat4 texCoordTransform = glm::mat4(1.0f);
	glm::vec3 texOffset(xOffset, yOffset, 0);
	texCoordTransform = glm::translate(texCoordTransform, texOffset);
	glm::vec3 texScale(xScale, yScale, 1);
	texCoordTransform = glm::scale(texCoordTransform, texScale);

	glUseProgram(shaderProgramID);

	unsigned int texCoordTransformLocation = glGetUniformLocation(shaderProgramID, "texTransform");
	glUniformMatrix4fv(texCoordTransformLocation, 1, GL_FALSE, glm::value_ptr(texCoordTransform));

	unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

	texture->bind();
	VAO->bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}