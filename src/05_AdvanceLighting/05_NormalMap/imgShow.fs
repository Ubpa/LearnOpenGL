#version 330 core
out vec4 FragColor;

in VS_OUT{
	vec2 TexCoords;
} fs_in;

uniform sampler2D texture0;

void main(){
	FragColor = vec4(vec3(texture(texture0, fs_in.TexCoords)), 1.0f);
}