#version 300 es
precision highp float;

layout (location = 0) in vec2 pos;

uniform mat4 coordTransform;

void main()
{
	gl_Position = coordTransform * vec4(pos.x, pos.y, 0.0, 1.0);
}