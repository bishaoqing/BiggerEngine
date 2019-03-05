#version 330 core
in vec2 ourUVCoord;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	FragColor = mix(texture(texture0, ourUVCoord), texture(texture1, ourUVCoord), 0.2);
};