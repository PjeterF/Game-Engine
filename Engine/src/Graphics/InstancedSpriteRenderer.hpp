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
	void addInstance(glm::vec2 position, glm::vec2 dimensions, float rotation, Texture* texture);
	void drawInstances();
	void reset();

	void setCamera(Camera* camera);
private:
	void assignTextureToUnit(int texId, int unit);

	Camera* camera;
	GLuint shaderProgramID;

	std::vector<int> unitIndices;
	std::unordered_map<int, int> texUnitMap;
	int currentFreeUnit = 0;

	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint elementBuffer;

	GLuint posBuf;
	GLuint dimBuf;
	GLuint rotBuf;
	GLuint texUnitBuf;

	std::vector<glm::vec2> positions;
	std::vector<glm::vec2> dimensions;
	std::vector<glm::vec4> rotations;
	std::vector<float> texUnits;
};