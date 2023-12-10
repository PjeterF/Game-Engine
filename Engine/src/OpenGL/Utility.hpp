#pragma once

#include <fstream>
#include <string>

const char* readFile(std::string filepath)
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