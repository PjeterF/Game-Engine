#include "ShaderProgram.hpp"

	ShaderProgram::ShaderProgram(std::string vertexSourceFilepath, std::string fragmentSourceFilepath)
	{
		const char* vertexSource = readFile(vertexSourceFilepath);
		const char* fragmentSource = readFile(fragmentSourceFilepath);

		//std::cout << vertexSource << std::endl << fragmentSource;

		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);

		GLint success = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
			std::cout << "Vertex shader errorlog:" << std::endl;
			for (GLchar c : infoLog)
			{
				std::cout << c;
			}
		}

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
			std::cout << "Fragment shader errorlog:" << std::endl;
			for (GLchar c : infoLog)
			{

				std::cout << c;
			}
		}

		id = glCreateProgram();

		glAttachShader(id, vertexShader);
		glAttachShader(id, fragmentShader);
		glLinkProgram(id);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void ShaderProgram::bind()
	{
		glUseProgram(id);
	}

	void ShaderProgram::unbind()
	{
		glUseProgram(0);
	}

	GLuint ShaderProgram::getId()
	{
		return id;
	}

	const char* ShaderProgram::readFile(std::string filepath)
	{
		std::string shaderSourceCode;
		std::string line = "";
		std::ifstream file(filepath);

		if (!file)
		{
			return nullptr;
		}
		else
		{
			while (!file.eof())
			{
				std::getline(file, line);
				shaderSourceCode.append(line + '\n');
			}

			file.close();

			char* retu = new char[shaderSourceCode.length() + 1];
			strcpy_s(retu, shaderSourceCode.length() + 1, shaderSourceCode.c_str());

			return retu;
		}
	}