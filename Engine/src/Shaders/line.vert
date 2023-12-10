#version 300 es
layout (location = 0) in vec2 vertPos;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(vertPos.x, vertPos.y, 0.0, 1.0);
}