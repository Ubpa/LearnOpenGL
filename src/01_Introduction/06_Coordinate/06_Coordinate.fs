#version 330 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	vec4 texColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.5);
	FragColor = mix( vertexColor, texColor, 0.5);
}