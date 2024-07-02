#version 300 es
precision highp float;

out vec4 finalColor;

in vec2 finalTexCoord;
flat in float texIndex;

uniform sampler2D samplers[32];

void main()
{
	finalColor=texture(samplers[int(texIndex)], finalTexCoord);
}