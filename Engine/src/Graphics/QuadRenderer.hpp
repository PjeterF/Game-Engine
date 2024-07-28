#pragma once

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <math.h>

#include "../OpenGL/VertexArray.hpp"
#include "../OpenGL/VertexBuffer.hpp"
#include "../OpenGL/ElementBuffer.hpp"
#include "../OpenGL/Texture.hpp"
#include "../Graphics/Camera.hpp"

class QuadRenderer
{
public:
	QuadRenderer(GLuint shaderProgramID, Camera* camera);
	void draw(glm::vec2 position, glm::vec2 scale, float angle, glm::vec4 colour);

	Camera* camera;
protected:
	VertexBuffer* VBO;
	VertexArray* VAO;
	ElementBuffer* EBO;
	GLuint shaderProgramID;
};