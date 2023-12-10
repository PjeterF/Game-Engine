#version 300 es
precision highp float;

out vec4 fragColor;

in vec2 fTexCoord;

uniform sampler2D texSampler;

uniform vec3 color;

void main()
{
	fragColor = texture(texSampler, fTexCoord);
	fragColor = vec4(color.x, color.y, color.z, 1.0) * fragColor;
}