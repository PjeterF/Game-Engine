#pragma once
#include "../OpenGL/package.hpp"
#include "glm/vec2.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLFW/glfw3.h"
#include <iostream>
#include <unordered_map>

#include "../Events/EventPackage.hpp"
#include "../Graphics/Camera.hpp"
#include "../Managers/Resource/ResourceManager.hpp"

#define SPRITE_RENDERER_MAX_TEX_SLOTS 8

class InstancedSpriteRenderer
{
public:
	InstancedSpriteRenderer(GLuint shaderProgramID, Camera* camera);
	void addInstance(glm::vec2 position, glm::vec2 dimensions, float rotation, Texture* texture, glm::vec4 textureSample = { 0, 0, 0, 0 });
	void drawInstances();

	void setCamera(Camera* camera);
private:
	class Batch
	{
	public:
		Batch(int index);
		void addInstance(glm::vec2 position, glm::vec2 dimensions, glm::vec4 rotTransform, Texture* texture, glm::vec4 textureSample = { 0, 0, 0, 0 });

		int index;
		std::unordered_map<int, int> texUnitMap;
		int currentFreeUnit = 0;

		std::vector<glm::vec2> positions;
		std::vector<glm::vec2> dimensions;
		std::vector<glm::vec4> rotTransforms;
		std::vector<float> texUnits;
		std::vector<glm::vec4> texTransforms;
	};

	void assignTextureToUnit(int texId, int unit);

	Camera* camera;
	GLuint shaderProgramID;

	std::vector<int> unitIndices;

	std::unordered_map<int, int> texBatchMap;
	std::vector<Batch> batches;

	std::unordered_map<int, int> texUnitMap;
	int currentFreeUnit = 0;

	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint elementBuffer;

	GLuint posBuf;
	GLuint dimBuf;
	GLuint rotBuf;
	GLuint texUnitBuf;
	GLuint texTransBuf;
};