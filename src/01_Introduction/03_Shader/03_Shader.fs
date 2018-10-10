#version 330 core
out vec4 FragColor;

in vec4 vertexColor; // �Ӷ�����ɫ�����������������������ͬ��������ͬ��
in vec4 anotherColor;

uniform vec4 ourColor; // ��OpenGL����������趨�������

void main()
{
    FragColor = (vertexColor + ourColor + anotherColor) / 3.0f;
}