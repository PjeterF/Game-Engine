#include <gladES/glad.h>
#include <vector>

	class ArrayBuffer
	{
	public:
		ArrayBuffer(std::vector<float>& vertices);
		void bind();
		void unbind();
		void bufferVertices(std::vector<float>& vertices);
	private:
		unsigned int id;
	};