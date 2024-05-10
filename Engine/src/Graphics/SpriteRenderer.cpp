#include "SpriteRenderer.hpp"

void SpriteRenderer::handleEvent(Event& event)
{
	switch (event.getType())
	{
	case Event::ZoomChange:
	{
		float* newZoom = (float*)event.getPayload();
		//this->setZoom(*newZoom);
	}
	break;
	case Event::CameraPan:
	{
		glm::vec2* pan = (glm::vec2*)event.getPayload();
	}
	break;
	}
}

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

	VBO = new ArrayBuffer(vertices);
	VBO->bind();

	EBO = new ElementArrayBuffer(indices);
	EBO->bind();

	VAO->setAttributePointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	VAO->setAttributePointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, sizeof(float) * 2);

	VAO->unbind();
}

void SpriteRenderer::draw(glm::vec2 position, glm::vec2 scale, float angle, Texture* texture)
{
	if (texture == nullptr)
		return;

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::ortho(camera->getOriginalFrustrumX().x, camera->getOriginalFrustrumX().y, camera->getOriginalFrustrumY().x, camera->getOriginalFrustrumY().y, -1.0f, 1.0f);
	transform = glm::translate(transform, glm::vec3(camera->getZoom() * position.x + camera->getOffset().x, camera->getZoom() * position.y + camera->getOffset().y, 0));
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

void SpriteRenderer::draw(glm::vec2 position, glm::vec2 scale, float angle, Texture* texture, TextureDivision division)
{
	if (texture == nullptr)
		return;

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::ortho(camera->getOriginalFrustrumX().x, camera->getOriginalFrustrumX().y, camera->getOriginalFrustrumY().x, camera->getOriginalFrustrumY().y, -1.0f, 1.0f);
	transform = glm::translate(transform, glm::vec3(camera->getZoom() * position.x + camera->getOffset().x, camera->getZoom() * position.y + camera->getOffset().y, 0));
	transform = glm::rotate(transform, glm::radians(angle + 0), glm::vec3(0, 0, 1));
	transform = glm::scale(transform, glm::vec3(camera->getZoom() * scale.x, camera->getZoom() * scale.y, 1));

	float xOffset = division.x / texture->getWidth();
	float yOffset = division.y / texture->getHeight();
	float xScale = division.width / texture->getWidth();
	float yScale = division.height / texture->getHeight();

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

	glActiveTexture(GL_TEXTURE0);
	texture->bind();
	VAO->bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	/*if (texture == nullptr)
		return;

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::ortho(camera->getOriginalFrustrumX().x, camera->getOriginalFrustrumX().y, camera->getOriginalFrustrumY().x, camera->getOriginalFrustrumY().y, -1.0f, 1.0f);
	transform = glm::translate(transform, glm::vec3(camera->getZoom() * position.x + camera->getOffset().x, camera->getZoom() * position.y + camera->getOffset().y, 0));
	transform = glm::rotate(transform, glm::radians(angle + 0), glm::vec3(0, 0, 1));
	transform = glm::scale(transform, glm::vec3(camera->getZoom() * scale.x, camera->getZoom() * scale.y, 1));

	float xOffset = division.x / texture->getWidth();
	float yOffset = division.y / texture->getHeight();
	float xScale = (division.width) / texture->getWidth();
	float yScale = (division.height) / texture->getHeight();

	glm::vec3 texOffset(0, 0, 0);
	glm::vec3 texScale(1, 1, 1);

	glm::mat4 translationM = glm::translate(glm::mat4(1.0f), texOffset);
	glm::mat4 scalingM = glm::scale(glm::mat4(1.0f), texScale);

	glm::mat4 texCoordTransform = glm::mat4(1.0f);
	texCoordTransform = glm::translate(texCoordTransform, texOffset);
	texCoordTransform = glm::scale(texCoordTransform, texScale);

	glUseProgram(shaderProgramID);

	unsigned int texCoordTransformLocation = glGetUniformLocation(shaderProgramID, "texTransform");
	glUniformMatrix4fv(texCoordTransformLocation, 1, GL_FALSE, glm::value_ptr(texCoordTransform));

	unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

	texture->bind();
	VAO->bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/

	//if (texture == nullptr)
	//	return;

	//glm::mat4 transform = glm::mat4(1.0f);
	//transform = glm::ortho(camera->getOriginalFrustrumX().x, camera->getOriginalFrustrumX().y, camera->getOriginalFrustrumY().x, camera->getOriginalFrustrumY().y, -1.0f, 1.0f);
	//transform = glm::translate(transform, glm::vec3(camera->getZoom() * position.x + camera->getOffset().x, camera->getZoom() * position.y + camera->getOffset().y, 0));
	//transform = glm::rotate(transform, glm::radians(angle + 0), glm::vec3(0, 0, 1));
	//transform = glm::scale(transform, glm::vec3(camera->getZoom() * scale.x, camera->getZoom() * scale.y, 1));

	//float xOffset = 0;
	//float yOffset = 0;
	//float xScale = 1;
	//float yScale = 1;

	//glm::mat4 texCoordTransform = glm::mat4(1.0f);
	//glm::vec3 texOffset(xOffset, yOffset, 0);
	//texCoordTransform = glm::translate(texCoordTransform, texOffset);
	//glm::vec3 texScale(xScale, yScale, 1);
	//texCoordTransform = glm::scale(texCoordTransform, texScale);

	//glUseProgram(shaderProgramID);

	//unsigned int texCoordTransformLocation = glGetUniformLocation(shaderProgramID, "texTransform");
	//glUniformMatrix4fv(texCoordTransformLocation, 1, GL_FALSE, glm::value_ptr(texCoordTransform));

	//unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "transform");
	//glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

	//texture->bind();
	//VAO->bind();
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
