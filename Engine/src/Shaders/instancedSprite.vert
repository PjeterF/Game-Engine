#version 300 es
precision highp float;

layout(location = 0) in vec2 modelPos;
layout(location = 2) in vec2 position;
layout(location = 3) in vec2 dimensions;

uniform mat4 projection;

void main()
{
	mat2 rot = mat2
	(
		rotation.xy,
		rotation.zw
	);

	int test = texUnit;

	vec2 finalPos = modelPos;
	finalPos = rot * finalPos;
	finalPos = (dimensions + 0. * float(texUnit)) * finalPos;
	finalPos = position + finalPos;

	gl_Position = projection * vec4(finalPos, 0.0, 1.0);
	finalTexCoord = texCoord;
}