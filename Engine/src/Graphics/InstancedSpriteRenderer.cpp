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

	batches.push_back(Batch(0));
}

void InstancedSpriteRenderer::addInstance(glm::vec2 position, glm::vec2 dimensions, float rotation, Texture* texture, glm::vec4 textureSample, bool flipHorizontally)
{
	if (texture == nullptr)
		return;

	if (!camera->isPointInFrustrum(position, 200))
		return;

	glm::vec4 texSample;
	if (glm::dot(textureSample, glm::vec4(1, 1, 1, 1)) == 0)
	{
		texSample = { 0, 0, 1, 1 };
	}
	else
	{
		texSample =
		{
			textureSample.x / texture->getWidth(),
			textureSample.y / texture->getHeight(),
			textureSample.z / texture->getWidth(),
			textureSample.w / texture->getHeight()
		};
	}

	auto batch_it = texBatchMap.find(texture->getId());
	//does tex belong to a batch
	if (batch_it == texBatchMap.end())
	{
		//can tex fit into current batch
		if (batches.back().texUnitMap.size() < SPRITE_RENDERER_MAX_TEX_SLOTS)
		{
			//add to current batch;
			batches.back().addInstance
			(
				{ position.x,position.y },
				dimensions,
				{ cos(rotation), -sin(rotation), sin(rotation), cos(rotation) },
				texture,
				texSample,
				flipHorizontally
			);
			texBatchMap[texture->getId()] = batches.back().index;
		}
		else
		{
			//create new batch and add
			batches.push_back(Batch(batches.size()));
			batches.back().addInstance
			(
				{ position.x,position.y },
				dimensions,
				{ cos(rotation), -sin(rotation), sin(rotation), cos(rotation) },
				texture,
				texSample,
				flipHorizontally
			);
			texBatchMap[texture->getId()] = batches.back().index;
		}
	}
	else
	{
		//tex belongs to a batch
		int batchIndex = (*batch_it).second;
		batches[batchIndex].addInstance
		(
			{ position.x,position.y },
			dimensions,
			{ cos(rotation), -sin(rotation), sin(rotation), cos(rotation) },
			texture,
			texSample,
			flipHorizontally
		);
	}
}

void InstancedSpriteRenderer::drawInstances()
{
	for (auto& batch : batches)
	{
		if (batch.positions.empty())
			continue;

		glBindBuffer(GL_ARRAY_BUFFER, posBuf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * batch.positions.size(), &batch.positions[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, dimBuf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * batch.dimensions.size(), &batch.dimensions[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, rotBuf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * batch.rotTransforms.size(), &batch.rotTransforms[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, texUnitBuf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * batch.texUnits.size(), &batch.texUnits[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, texTransBuf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * batch.texTransforms.size(), &batch.texTransforms[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glUseProgram(shaderProgramID);

		glm::mat4 projection = glm::ortho(camera->getFrustrumX().x, camera->getFrustrumX().y, camera->getFrustrumY().x, camera->getFrustrumY().y, -1.0f, 1.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		for (auto var : batch.texUnitMap)
		{
			glActiveTexture(GL_TEXTURE0 + var.second);
			glBindTexture(GL_TEXTURE_2D, var.first);
		}

		glUniform1iv(glGetUniformLocation(shaderProgramID, "samplers"), unitIndices.size(), &unitIndices[0]);

		glBindVertexArray(vertexArray);
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, batch.positions.size());
		glBindVertexArray(0);
	}

	batches.clear();
	texBatchMap.clear();

	batches.push_back(Batch(0));
}

void InstancedSpriteRenderer::setCamera(Camera* camera)
{
	this->camera = camera;
}

InstancedSpriteRenderer::Batch::Batch(int index) : index(index)
{
}

void InstancedSpriteRenderer::Batch::addInstance(glm::vec2 position, glm::vec2 dimensions, glm::vec4 rotTransform, Texture* texture, glm::vec4 textureSample, bool flipHorizontally)
{
	if (flipHorizontally)
	{
		textureSample.x = textureSample.x + textureSample.z;
		textureSample.z = -textureSample.z;
	}

	int texId = texture->getId();
	auto mapEntry = texUnitMap.find(texId);
	if (mapEntry == texUnitMap.end())
	{
		texUnitMap[texId] = currentFreeUnit;
		this->texUnits.push_back(currentFreeUnit);
		currentFreeUnit++;
	}
	else
	{
		this->texUnits.push_back((*mapEntry).second);
	}

	this->positions.push_back(position);
	this->dimensions.push_back(dimensions);
	this->rotTransforms.push_back(rotTransform);
	this->texTransforms.push_back(textureSample);
}
