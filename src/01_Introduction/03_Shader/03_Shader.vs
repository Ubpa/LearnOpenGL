#version 330 core
layout (location = 0) in vec3 aPos;// 位置变量的属性位置为0
layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1

out vec4 vertexColor; // 为片段着色器指定一个颜色输出
out vec4 anotherColor;

uniform vec3 offset;

void main()
{
    gl_Position = vec4(aPos + offset, 1.0);
	vertexColor = vec4(0.5, 0.0, 0.0, 1.0);// 暗红色
	anotherColor = vec4(aColor, 1.0);
}