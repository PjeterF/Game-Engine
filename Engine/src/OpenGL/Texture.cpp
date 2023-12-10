#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../../Externals/stb_image.h"

Texture::Texture(std::string texturePath, bool flip)
{
	this->filepath = texturePath;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int comp = 3;
	if (texturePath.find(".png") != std::string::npos)
	{
		comp = 4;
	}
	this->components = comp;

	int nrChannels;
	if (flip)
	{
		stbi_set_flip_vertically_on_load(true);
	}
	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, comp);
	if (data)
	{
		if (comp == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		glDeleteTextures(1, &texture);
	}

	stbi_image_free(data);
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &texture);
}