#include "QuadRenderer.hpp"

QuadRenderer::QuadRenderer(GLuint shaderProgramID, Camera* camera)
{
	this->shaderProgramID = shaderProgramID;
	this->camera = camera;

	std::vector<float> vertices;
	vertices.push_back(-1);
	vertices.push_back(1);

	vertices.push_back(1);
	vertices.push_back(1);

	vertices.push_back(-1);
	vertices.push_back(-1);

	vertices.push_back(1);
	vertices.push_back(-1);

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

	VAO->setAttributePointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	VAO->unbind();
}

void QuadRenderer::draw(glm::vec2 position, glm::vec2 scale, float angle, glm::vec4 colour)
{
	if (!camera->isPointInFrustrum(position, 50))
		return;

	glm::mat4 coordTransform = glm::mat4(1.0f);
	coordTransform = glm::ortho(camera->getOriginalFrustrumX().x, camera->getOriginalFrustrumX().y, camera->getOriginalFrustrumY().x, camera->getOriginalFrustrumY().y, -1.0f, 1.0f);
	coordTransform = glm::translate(coordTransform, glm::vec3(camera->getZoom() * position.x - camera->getPosition().x, camera->getZoom() * position.y - camera->getPosition().y, 0));
	coordTransform = glm::rotate(coordTransform, glm::radians(angle + 0), glm::vec3(0, 0, 1));
	coordTransform = glm::scale(coordTransform, glm::vec3(camera->getZoom() * scale.x, camera->getZoom() * scale.y, 1));
	
	glUseProgram(shaderProgramID);

	unsigned int coordTransformLocation = glGetUniformLocation(shaderProgramID, "coordTransform");
	glUniformMatrix4fv(coordTransformLocation, 1, GL_FALSE, glm::value_ptr(coordTransform));

	unsigned int colourLocation = glGetUniformLocation(shaderProgramID, "colour");
	glUniform4f(colourLocation, colour.x, colour.y, colour.z, colour.w);

	VAO->bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
}

void QuadRenderer::handleEvent(Event& event)
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
