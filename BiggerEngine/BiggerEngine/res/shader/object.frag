#version 330 core
in vec3 fragNormal;
in vec3 fragPos;

out vec4 FragColor;

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

struct Light{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

uniform vec3 viewPos;

void main()
{
	vec3 ambient = light.ambient * material.ambient;

	vec3 normal = normalize(fragNormal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0);
	vec3 diffuse = (diff * material.diffuse) * light.diffuse;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
	vec3 specular = (spec * material.specular)  * light.specular;

	vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}