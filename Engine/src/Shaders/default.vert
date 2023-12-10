#version 300 es
layout (location = 0) in vec2 vertPos;

void main()
{
	gl_Position = vec4(vertPos.x, vertPos.y, 0.0, 1.0);
}