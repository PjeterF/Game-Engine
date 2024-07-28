#include "LineRenderer.hpp"

LineRenderer::LineRenderer(GLuint shaderProgramID, Camera* camera)
{
	this->camera = camera;
	this->shaderProgramID = shaderProgramID;

	std::vector<float> vertices;
	vertices.push_back(-1);
	vertices.push_back(0);
	vertices.push_back(1);
	vertices.push_back(0);

	VAO = new VertexArray;
	VAO->bind();

	VBO = new VertexBuffer(vertices);
	VBO->bind();

	VAO->setAttributePointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	VAO->unbind();
}

void LineRenderer::draw(float x1, float y1, float x2, float y2, float width, float r, float g, float b)
{
	glm::vec2 vec(x1 - x2, y1 - y2);
	float length = 0.5*glm::length(vec);
	glLineWidth(camera->getZoom() * width);

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::ortho(camera->getOriginalFrustrumX().x, camera->getOriginalFrustrumX().y, camera->getOriginalFrustrumY().x, camera->getOriginalFrustrumY().y, -1.0f, 1.0f);
	transform = glm::translate(transform, glm::vec3(camera->getZoom() * (0.5 * (x1+x2) - camera->getPosition().x), camera->getZoom() * (0.5 * (y1 + y2) - camera->getPosition().y), 0));
	transform = glm::rotate(transform, atan(vec.y/vec.x), glm::vec3(0, 0, 1));
	transform = glm::scale(transform, glm::vec3(camera->getZoom() * length, camera->getZoom() * length, 1));

	glUseProgram(shaderProgramID);
	unsigned int transformLocation = glGetUniformLocation(shaderProgramID, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

	unsigned int color = glGetUniformLocation(shaderProgramID, "color");
	glUniform3f(color, r, g, b);

	VAO->bind();
	glDrawArrays(GL_LINES, 0, 2);
	VAO->unbind();
}