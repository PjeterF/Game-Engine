#pragma once
#include <gladES/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

	class ShaderProgram
	{
	public:
		ShaderProgram(std::string vertexSourceFilepath, std::string fragmentSourceFilepath);
		void bind();
		void unbind();
		GLuint getId();
	private:
		const char* readFile(std::string filepath);
		GLuint id;
	};