#version 300 es
precision highp float;

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoord;

out vec2 fTexCoord;

uniform mat4 transform;
uniform mat4 texTransform;

void main()
{
	gl_Position = transform * vec4(pos.x, pos.y, 0.0, 1.0);
	vec4 temp = texTransform * vec4(texCoord.x, texCoord.y, 0.0, 1.0);
	fTexCoord = vec2(temp.x, temp.y);
}