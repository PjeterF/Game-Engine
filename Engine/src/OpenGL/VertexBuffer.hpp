#include <gladES/glad.h>
#include <vector>

	class VertexBuffer
	{
	public:
		VertexBuffer(std::vector<float>& vertices);
		void bind();
		void unbind();
		void bufferVertices(std::vector<float>& vertices);
	private:
		unsigned int id;
	};