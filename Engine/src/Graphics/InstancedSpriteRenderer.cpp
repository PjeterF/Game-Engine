#include "InstancedSpriteRenderer.hpp"

InstancedSpriteRenderer::InstancedSpriteRenderer(GLuint shaderProgramID, Camera* camera) : shaderProgramID(shaderProgramID), camera(camera)
{
	std::vector<float> vertices =
	{
		-1, 1, 0, 1,
		1, 1, 1, 1,
		-1, -1, 0, 0,
		1, -1, 1, 0
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

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &posBuf);
	glGenBuffers(1, &dimBuf);
	glGenBuffers(1, &rotBuf);
	glGenBuffers(1, &texUnitBuf);
	glGenBuffers(1, &texTransBuf);

	glBindBuffer(GL_ARRAY_BUFFER, posBuf);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	glBindBuffer(GL_ARRAY_BUFFER, dimBuf);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);

	glBindBuffer(GL_ARRAY_BUFFER, rotBuf);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 1);

	glBindBuffer(GL_ARRAY_BUFFER, texUnitBuf);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(5, 1);

	glBindBuffer(GL_ARRAY_BUFFER, texTransBuf);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	glEnableVertexAttribArray(6);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);

	for (int i = 0; i < SPRITE_RENDERER_MAX_TEX_SLOTS; i++)
		unitIndices.push_back(i);
}

void InstancedSpriteRenderer::addInstance(glm::vec2 position, glm::vec2 dimensions, float rotation, Texture* texture, glm::vec4 textureSample)
{
	if (texture == nullptr)
		return;

	this->positions.push_back({ camera->getOffset().x + camera->getZoom() * position.x, camera->getOffset().y + camera->getZoom() * position.y });
	this->dimensions.push_back(camera->getZoom() * dimensions);
	this->rotTransforms.push_back({ cos(rotation), -sin(rotation), sin(rotation), cos(rotation) });

	int texId = texture->getId();
	auto mapEntry = texUnitMap.find(texId);
	if (mapEntry == texUnitMap.end())
	{
		if (texUnitMap.size() < SPRITE_RENDERER_MAX_TEX_SLOTS)
		{
			assignTextureToUnit(texId, currentFreeUnit);
			this->texUnits.push_back(currentFreeUnit);
			currentFreeUnit++;
		}
		else
		{
			throw "No more slots";
		}
	}
	else
	{
		this->texUnits.push_back((*mapEntry).second);
	}

	if (glm::dot(textureSample, glm::vec4(1, 1, 1, 1))==0)
	{
		texTransforms.push_back({ 0, 0, 1, 1 });
	}
	else
	{
		texTransforms.push_back
		({
			textureSample.x / texture->getWidth(),
			textureSample.y / texture->getHeight(),
			textureSample.z / texture->getWidth(),
			textureSample.w / texture->getHeight()
			});
	}

	
}

void InstancedSpriteRenderer::drawInstances()
{
	if (positions.empty())
		return;

	glBindBuffer(GL_ARRAY_BUFFER, posBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * positions.size(), &positions[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, dimBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * dimensions.size(), &dimensions[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, rotBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * rotTransforms.size(), &rotTransforms[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, texUnitBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texUnits.size(), &texUnits[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, texTransBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * texTransforms.size(), &texTransforms[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(shaderProgramID);

	glm::mat4 projection = glm::ortho(camera->getOriginalFrustrumX().x, camera->getOriginalFrustrumX().y, camera->getOriginalFrustrumY().x, camera->getOriginalFrustrumY().y, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	for (auto var : texUnitMap)
	{
		glActiveTexture(GL_TEXTURE0 + var.second);
		glBindTexture(GL_TEXTURE_2D, var.first);
	}

	glUniform1iv(glGetUniformLocation(shaderProgramID, "samplers"), unitIndices.size(), &unitIndices[0]);

	glBindVertexArray(vertexArray);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, positions.size());
	glBindVertexArray(0);

	this->reset();
}

void InstancedSpriteRenderer::reset()
{
	positions.clear();
	dimensions.clear();
	rotTransforms.clear();
	texUnits.clear();
	texTransforms.clear();

	currentFreeUnit = 0;
	texUnitMap.clear();
}

void InstancedSpriteRenderer::setCamera(Camera* camera)
{
	this->camera = camera;
}

void InstancedSpriteRenderer::assignTextureToUnit(int texId, int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texId);
	texUnitMap[texId] = unit;
}

InstancedSpriteRenderer::Batch::Batch(int index) : index(index)
{
}

void InstancedSpriteRenderer::Batch::addInstance(glm::vec2 position, glm::vec2 dimensions, float rotation, Texture* texture, glm::vec4 textureSample)
{
	if (texture == nullptr)
		return;

	this->positions.push_back(position);
	this->dimensions.push_back(dimensions);
	this->rotTransforms.push_back({ cos(rotation), -sin(rotation), sin(rotation), cos(rotation) });

	int texId = texture->getId();
	auto mapEntry = texUnitMap.find(texId);
	if (mapEntry == texUnitMap.end())
	{
		if (texUnitMap.size() < SPRITE_RENDERER_MAX_TEX_SLOTS)
		{
			texUnitMap[texId] = currentFreeUnit;
			this->texUnits.push_back(currentFreeUnit);
			currentFreeUnit++;
		}
		else
		{
			throw "No more slots";
		}
	}
	else
	{
		this->texUnits.push_back((*mapEntry).second);
	}

	if (glm::dot(textureSample, glm::vec4(1, 1, 1, 1)) == 0)
	{
		texTransforms.push_back({ 0, 0, 1, 1 });
	}
	else
	{
		texTransforms.push_back
		({
			textureSample.x / texture->getWidth(),
			textureSample.y / texture->getHeight(),
			textureSample.z / texture->getWidth(),
			textureSample.w / texture->getHeight()
			});
	}
}
