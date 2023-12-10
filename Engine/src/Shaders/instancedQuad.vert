#version 300 es
precision highp float;

layout(location = 0) in vec2 modelPos;
layout(location = 1) in vec2 positionOffset;
layout(location = 2) in float size;
layout(location = 3) in vec4 color;
layout(location = 4) in float angle;

uniform mat4 projection;

out vec4 fragColor;

void main()
{
	mat2 rotation = mat2
	(
		vec2(cos(angle), -sin(angle)),
		vec2(sin(angle), cos(angle))
	);

	gl_Position = projection * vec4(rotation * size * modelPos + positionOffset, 0.0, 1.0);
	fragColor = color;
}