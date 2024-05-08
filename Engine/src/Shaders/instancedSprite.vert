#version 300 es
precision highp float;

layout(location = 0) in vec2 modelPos;
layout(location = 2) in vec2 position;
layout(location = 3) in vec2 dimensions;

uniform mat4 projection;

void main()
{
	gl_Position = projection * vec4(dimensions.x * modelPos + position, 0.0, 1.0);
}