#pragma once

#include "../OpenGL/package.hpp"
#include "glm/vec2.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLFW/glfw3.h"
#include <iostream>
#include <math.h>

#include "../Events/EventPackage.hpp"
#include "../Graphics/Camera.hpp"
class TextRenderer
{
public:
	TextRenderer(GLuint shaderProgramID, Camera* camera, Texture* bitmapFont, int birmapSizeHorizontal, int birmapSizeVertical);
	void draw(float x, float y, float size, char character);
	void draw(float x, float y, float size, std::string str);
	void setColor(float r, float g, float b);
private:
	VertexBuffer* VBO;
	VertexArray* VAO;
	ElementBuffer* EBO;
	GLuint shaderProgramID;
	Camera* camera;
	Texture* bitmapFont;
	glm::vec3 color = glm::vec3(1, 1, 1);

	int birmapSizeHorizontal;
	int birmapSizeVertical;
};

