#version 330 core
out vec4 color;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;

uniform int mode;
uniform float exposure;

void main()
{
	float gamma = 2.2f;

    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
	vec3 mapped;
    if(mode == 1){
		// Reinhard色调映射
		mapped = hdrColor / (hdrColor + vec3(1.0/exposure));
	}else if(mode == 2){
		// 曝光色调映射
		mapped = vec3(1.0) - exp(-hdrColor * exposure);
	}else{
		mapped = hdrColor;
	}
	
	mapped = pow(mapped, vec3(1.0 / gamma));
	color = vec4(mapped, 1.0);
}