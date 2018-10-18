#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
	sampler2D emission;
    float shininess;//高光大小
};

struct Light {
    vec3 position;
    vec3 direction;
	float cutoff;
	float outerCutoff;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;
uniform vec3 lightColor;
uniform vec3 viewPos;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

void main()
{
    // 环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = vec3(0,0,0);
	vec3 specular = vec3(0,0,0);
	vec3 lightDir = normalize(light.position - FragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

	if(intensity > 0) 
	{// 执行光照计算

		// 漫反射 
		vec3 norm = normalize(Normal);
		//vec3 lightDir = normalize(-light.direction);
		float diff = max(dot(norm, lightDir), 0.0);
		diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

		// 镜面光
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	}

	float distance    = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 result = ambient + attenuation * intensity *(diffuse + specular) + attenuation * vec3(texture(material.emission, TexCoords));
	
    FragColor = vec4(result, 1.0);
}