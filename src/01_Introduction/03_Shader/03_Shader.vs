#version 330 core
layout (location = 0) in vec3 aPos;// λ�ñ���������λ��Ϊ0
layout (location = 1) in vec3 aColor; // ��ɫ����������λ��ֵΪ 1

out vec4 vertexColor; // ΪƬ����ɫ��ָ��һ����ɫ���
out vec4 anotherColor;

uniform vec3 offset;

void main()
{
    gl_Position = vec4(aPos + offset, 1.0);
	vertexColor = vec4(0.5, 0.0, 0.0, 1.0);// ����ɫ
	anotherColor = vec4(aColor, 1.0);
}