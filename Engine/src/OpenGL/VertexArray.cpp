#include "VertexArray.hpp"

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &id);
		glBindVertexArray(id);
	}

	void VertexArray::bind()
	{
		glBindVertexArray(id);
	}

	void VertexArray::unbind()
	{
		glBindVertexArray(0);
	}

	void VertexArray::setAttributePointer(int location, int componentsPerVertex, unsigned int type, bool normalized, int stride, int offset)
	{
		glBindVertexArray(id);
		glVertexAttribPointer(location, componentsPerVertex, type, normalized, stride, (void*)offset);
		glEnableVertexAttribArray(location);
	}