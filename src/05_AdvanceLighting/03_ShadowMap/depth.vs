#version 330 core
layout (location = 0) in vec3 aPos;

layout (std140) uniform LightMatrix{
	mat4 lightSpaceMatrix;
};

uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}