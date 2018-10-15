#version 330 core
out vec4 FragColor;

in vec4 vertexColor; // 从顶点着色器传来的输入变量（名称相同、类型相同）
in vec4 anotherColor;
in vec2 TexCoord;
uniform vec4 ourColor; // 在OpenGL程序代码中设定这个变量

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    vec4 color0 = (vertexColor + ourColor + anotherColor) / 3.0f;
	vec4 color1 = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.5);
	FragColor = mix(color0, color1, 0.5);
}