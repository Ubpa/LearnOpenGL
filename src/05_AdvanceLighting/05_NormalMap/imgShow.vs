#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

layout (std140) uniform CameraMatrixs{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

out VS_OUT{
	vec2 TexCoords;
} vs_out;

void main(){
	vs_out.TexCoords = aTexCoords;

	gl_Position = projection * view * model * vec4(aPos,1.0);
}