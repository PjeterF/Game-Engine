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
#include "Animation.hpp"

class SpriteRenderer : public EventObserver
{
public:
	SpriteRenderer(GLuint shaderProgramID, Camera* camera);
	void draw(glm::vec2 position, glm::vec2 scale, float angle, Texture* texture);
	void draw(glm::vec2 position, glm::vec2 scale, float angle, Texture* texture, Animation::Division division);

	virtual void handleEvent(Event& event) override;

	Camera* camera;
protected:
	VertexBuffer* VBO;
	VertexArray* VAO;
	ElementBuffer* EBO;
	GLuint shaderProgramID;

};