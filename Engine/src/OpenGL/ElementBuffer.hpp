#pragma once

#include <gladES/glad.h>
#include <vector>

	class ElementBuffer
	{
	public:
		ElementBuffer(std::vector<int>& indices);
		void bind();
		void unbind();
		void bufferIndices(std::vector<int>& indices);
	private:
		unsigned int id;
	};