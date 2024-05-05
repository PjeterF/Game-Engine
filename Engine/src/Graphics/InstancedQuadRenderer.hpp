#include "../OpenGL/package.hpp"
#include "glm/vec2.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLFW/glfw3.h"
#include <iostream>

#include "../Events/EventPackage.hpp"
#include "../Graphics/Camera.hpp"

class InstancedQuadRenderer
{
public:
	InstancedQuadRenderer(GLuint shaderProgramID, Camera* camera);
	~InstancedQuadRenderer();
	void commisionInstance(float xPos, float yPos, float scale, float angle, float r, float g, float b, float a);
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
		void addInstance(float xPos, float yPos, float scale, float angle, float r, float g, float b, float a);
		void reset();
		int size();
		bool empty();

		std::vector<glm::vec2> positions;
		std::vector<float> sizes;
		std::vector<glm::vec4> colors;
		std::vector<float> angles;

		Camera* camera = nullptr;
	private:
		int currentIndex = 0;
		int bufferSize = 50000;
	};

	ArrayBuffer* VBO;
	VertexArray* VAO;
	ElementArrayBuffer* EBO;
	GLuint shaderProgramID;

	INSTANCES_DATA instanceData;

	unsigned int instancePos;
	unsigned int instanceSize;
	unsigned int instanceColor;
	unsigned int instanceAngle;
};