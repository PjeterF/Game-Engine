#pragma once

#include <gladES/glad.h>

	class VertexArray
	{
	public:
		VertexArray();
		void bind();
		void unbind();
		void setAttributePointer(int location, int componentsPerVertex, unsigned int type, bool normalized, int stride, int offset);
	private:
		unsigned int id;
	};