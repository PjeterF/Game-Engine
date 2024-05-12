#version 300 es
precision highp float;

layout(location = 0) in vec2 modelPos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec2 position;
layout(location = 3) in vec2 dimensions;
layout(location = 4) in vec4 rotation;
layout(location = 5) in float texUnit;
layout(location = 6) in vec4 texTransform;

out vec2 finalTexCoord;
flat out float texIndex;

uniform mat4 projection;

void main()
{
	texIndex=texUnit;
	
	mat2 rot = mat2
	(
		rotation.xy,
		rotation.zw
	);

	vec2 finalPos = modelPos;
	finalPos = rot * finalPos;
	finalPos = dimensions * finalPos;
	finalPos = position + finalPos;

	gl_Position = projection * vec4(finalPos, 0.0, 1.0);
	finalTexCoord = vec2(texCoord.x*texTransform.z + texTransform.x, texCoord.y*texTransform.w + texTransform.y);
}