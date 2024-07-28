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

class LineRenderer
{
public:
	LineRenderer(GLuint shaderProgramID, Camera* camera);
	void draw(float x1, float y1, float x2, float y2, float width, float r, float g, float b);

	Camera* camera;
protected:
	VertexBuffer* VBO;
	VertexArray* VAO;
	GLuint shaderProgramID;

};

