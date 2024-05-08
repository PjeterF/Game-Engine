#pragma once
#include "../OpenGL/package.hpp"
#include "glm/vec2.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLFW/glfw3.h"
#include <iostream>

#include "../Events/EventPackage.hpp"
#include "../Graphics/Camera.hpp"

class InstancedSpriteRenderer
{
public:
	InstancedSpriteRenderer(GLuint shaderProgramID, Camera* camera);
	void addInstance(glm::vec2 position, glm::vec2 dimensions);
	void drawInstances();
	void reset();
private:
	Camera* camera;
	GLuint shaderProgramID;

	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint elementBuffer;

	GLuint posBuf;
	GLuint dimBuf;

	std::vector<glm::vec2> positions;
	std::vector<glm::vec2> dimensions;
	std::vector<float> rotations;
	std::vector<int> texUnitIndex;
};