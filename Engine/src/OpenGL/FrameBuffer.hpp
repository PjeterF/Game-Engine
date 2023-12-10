#pragma once
#include <gladES/glad.h>
#include <iostream>
class FrameBuffer
{
public:
	FrameBuffer(int width, int height);  
	void bind();
	void unbind();
	unsigned int getTextureID() { return texture; }
	void bindTexture() { glBindTexture(GL_TEXTURE_2D, texture); }
private:
	unsigned int frameBuffer;
	unsigned int texture;
};

