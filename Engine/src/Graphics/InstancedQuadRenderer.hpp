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

class InstancedQuadRenderer
{
public:
	InstancedQuadRenderer(GLuint shaderProgramID, Camera* camera);
	~InstancedQuadRenderer();
	void commisionInstance(float xPos, float yPos, float scaleX, float scaleY, float angle, float r, float g, float b, float a);
	void drawInstances();
	Camera* getCamera();

	Camera* camera;
private:
	class INSTANCES_DATA
	{
	public:
		INSTANCES_DATA(Camera* camera);
		INSTANCES_DATA();
		void setBufferSize(int size);
		int getBufferSize();
		void addInstance(float xPos, float yPos, float scaleX, float scaleY, float angle, float r, float g, float b, float a);
		void reset();
		int size();
		bool empty();

		std::vector<glm::vec2> positions;
		std::vector<glm::vec2> sizes;
		std::vector<glm::vec4> colors;
		std::vector<float> angles;

		Camera* camera = nullptr;
	private:
		int currentIndex = 0;
		int bufferSize = 100000;
	};

	VertexBuffer* VBO;
	VertexArray* VAO;
	ElementBuffer* EBO;
	GLuint shaderProgramID;

	INSTANCES_DATA instanceData;

	unsigned int instancePos;
	unsigned int instanceSize;
	unsigned int instanceColor;
	unsigned int instanceAngle;
};