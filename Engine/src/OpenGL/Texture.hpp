#pragma once
#include <gladES/glad.h>
#include <iostream>

class Texture
{
public:
	Texture(std::string texturePath, bool flip = true);
	~Texture();
	void bind();
	void unbind();
	int getWidth() { return width; }
	int getHeight() { return height; }
	int getNoComponents() { return components; }
	GLuint getId() { return texture; }
	std::string getFilepath() { return filepath; }
private:
	GLuint texture;
	std::string filepath;
	int width;
	int height;
	int components;
};