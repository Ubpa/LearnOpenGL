#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 offset;

out vec2 TexCoords;

uniform mat4 model;

layout (std140) uniform CameraMatrixs{
	mat4 view;
	mat4 projection;
};

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos + offset, 1.0);
}