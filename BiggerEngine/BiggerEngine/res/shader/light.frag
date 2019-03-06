#version 330 core
in vec2 UV;

uniform sampler2D sampler0;

out vec4 FragColor;

void main()
{
    FragColor = texture(sampler0, UV);
}