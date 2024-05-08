#include "InstancedSpriteRenderer.hpp"

InstancedSpriteRenderer::InstancedSpriteRenderer(GLuint shaderProgramID, Camera* camera) : shaderProgramID(shaderProgramID), camera(camera)
{
	std::vector<float> vertices =
	{
		-1, 1,
		1, 1,
		-1, -1,
		1, -1
	};

	std::vector<int> indices =
	{
		0, 1, 2,
		1, 2, 3
	};

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);

	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &posBuf);
	glGenBuffers(1, &dimBuf);

	glBindBuffer(GL_ARRAY_BUFFER, posBuf);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	glBindBuffer(GL_ARRAY_BUFFER, dimBuf);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);

	glBindVertexArray(0);
}

void InstancedSpriteRenderer::addInstance(glm::vec2 position, glm::vec2 dimensions)
{
	this->positions.push_back({ camera->getOffset().x + camera->getZoom() * position.x, camera->getOffset().y + camera->getZoom() * position.y });
	this->dimensions.push_back(camera->getZoom() * dimensions);
}

void InstancedSpriteRenderer::drawInstances()
{
	if (positions.empty())
		return;

	glBindBuffer(GL_ARRAY_BUFFER, posBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * positions.size()*2, &positions[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, dimBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * dimensions.size()*2, &dimensions[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(shaderProgramID);

	glm::mat4 projection = glm::ortho(camera->getOriginalFrustrumX().x, camera->getOriginalFrustrumX().y, camera->getOriginalFrustrumY().x, camera->getOriginalFrustrumY().y, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(vertexArray);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, positions.size());
	glBindVertexArray(0);

	this->reset();
}

void InstancedSpriteRenderer::reset()
{
	positions.clear();
	dimensions.clear();
}
