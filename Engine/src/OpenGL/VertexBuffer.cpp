#include "VertexBuffer.hpp"

	ArrayBuffer::ArrayBuffer(std::vector<float>& vertices)
	{
		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
	}

	void ArrayBuffer::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);

	}

	void ArrayBuffer::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	void ArrayBuffer::bufferVertices(std::vector<float>& vertices)
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
	}