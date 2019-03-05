#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vec3Color;
layout (location = 2) in vec2 uvCoord;

uniform mat4 transform;

out vec3 ourColor;
out vec2 ourUVCoord;


void main()
{
   gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   ourUVCoord = uvCoord;
   ourColor = vec3Color;
}