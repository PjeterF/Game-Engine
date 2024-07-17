#include "InstancedQuadRenderer.hpp"

InstancedQuadRenderer::InstancedQuadRenderer(GLuint shaderProgramID, Camera* camera)
{
	instanceData = INSTANCES_DATA(camera);

	this->camera = camera;
	this->shaderProgramID = shaderProgramID;

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

	VAO = new VertexArray;
	VAO->bind();

	VBO = new ArrayBuffer(vertices);
	VBO->bind();

	EBO = new ElementArrayBuffer(indices);
	EBO->bind();

	VAO->setAttributePointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	glGenBuffers(1, &instancePos);
	glGenBuffers(1, &instanceSize);
	glGenBuffers(1, &instanceColor);
	glGenBuffers(1, &instanceAngle);

	glBindBuffer(GL_ARRAY_BUFFER, instancePos);
	VAO->setAttributePointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glVertexAttribDivisor(1, 1);

	glBindBuffer(GL_ARRAY_BUFFER, instanceSize);
	VAO->setAttributePointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glVertexAttribDivisor(2, 1);

	glBindBuffer(GL_ARRAY_BUFFER, instanceColor);
	VAO->setAttributePointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glVertexAttribDivisor(3, 1);

	glBindBuffer(GL_ARRAY_BUFFER, instanceAngle);
	VAO->setAttributePointer(4, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), 0);
	glVertexAttribDivisor(4, 1);

	VAO->unbind();
}

InstancedQuadRenderer::~InstancedQuadRenderer()
{
	glDeleteBuffers(1, &instancePos);
	glDeleteBuffers(1, &instanceSize);
	glDeleteBuffers(1, &instanceColor);
	glDeleteBuffers(1, &instanceAngle);
}

void InstancedQuadRenderer::commisionInstance(float xPos, float yPos, float scaleX, float scaleY, float angle, float r, float g, float b, float a)
{
	instanceData.addInstance(xPos, yPos, scaleX, scaleY, angle, r, g, b, a);
}

void InstancedQuadRenderer::drawInstances()
{
	if (instanceData.empty())
		return;

	glBindBuffer(GL_ARRAY_BUFFER, instancePos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * instanceData.size(), &instanceData.positions[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, instanceSize);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * instanceData.size(), &instanceData.sizes[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, instanceColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * instanceData.size(), &instanceData.colors[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, instanceAngle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * instanceData.size(), &instanceData.angles[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(shaderProgramID);

	glm::mat4 projection = glm::ortho(camera->getFrustrumX().x, camera->getFrustrumX().y, camera->getFrustrumY().x, camera->getFrustrumY().y, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	VAO->bind();
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, instanceData.size());
	VAO->unbind();

	instanceData.reset();
}

Camera* InstancedQuadRenderer::getCamera()
{
	return camera;
}

InstancedQuadRenderer::INSTANCES_DATA::INSTANCES_DATA(Camera* camera)
{
	positions.resize(bufferSize);
	sizes.resize(bufferSize);
	colors.resize(bufferSize);
	angles.resize(bufferSize);

	this->camera = camera;
}

InstancedQuadRenderer::INSTANCES_DATA::INSTANCES_DATA()
{
	positions.resize(bufferSize);
	sizes.resize(bufferSize);
	colors.resize(bufferSize);
	angles.resize(bufferSize);
}

void InstancedQuadRenderer::INSTANCES_DATA::setBufferSize(int size)
{
	positions.clear();
	sizes.clear();
	colors.clear();
	angles.clear();

	positions.resize(bufferSize);
	sizes.resize(bufferSize);
	colors.resize(bufferSize);
	angles.resize(bufferSize);

	bufferSize = size;
}

int InstancedQuadRenderer::INSTANCES_DATA::getBufferSize()
{
	return bufferSize;
}

void InstancedQuadRenderer::INSTANCES_DATA::addInstance(float xPos, float yPos, float scaleX, float scaleY, float angle, float r, float g, float b, float a)
{
	if (currentIndex >= bufferSize)
		return;

	/*if (!camera->isPointInFrustrum({xPos, yPos}, -50))
		return;*/

	positions[currentIndex] = { xPos, yPos };
	sizes[currentIndex] = {scaleX, scaleY};
	colors[currentIndex] = { r, g, b, a };
	angles[currentIndex] = angle;

	currentIndex++;
}

void InstancedQuadRenderer::INSTANCES_DATA::reset()
{
	currentIndex = 0;
}

int InstancedQuadRenderer::INSTANCES_DATA::size()
{
	return currentIndex;
}

bool InstancedQuadRenderer::INSTANCES_DATA::empty()
{
	if (currentIndex == 0)
		return true;
	else
		return false;
}
