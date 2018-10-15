#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 vertexColor;
out vec4 anotherColor;
out vec2 TexCoord;

uniform vec3 offset;

void main()
{
    gl_Position = vec4(aPos + offset, 1.0);
	vertexColor = vec4(0.5, 0.0, 0.0, 1.0);// °µºìÉ«
	anotherColor = vec4(aColor, 1.0);
	TexCoord = aTexCoord;
}