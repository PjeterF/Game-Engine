#version 300 es
precision highp float;

out vec4 fragColor;

in vec2 fTexCoord;

uniform sampler2D texSampler;

void main()
{
	fragColor = texture(texSampler, fTexCoord);
}