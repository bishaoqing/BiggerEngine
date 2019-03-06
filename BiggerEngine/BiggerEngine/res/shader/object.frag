#version 330 core
in vec3 fragNormal;
in vec3 fragPos;
in vec2 texCoord;

out vec4 FragColor;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
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
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));

	vec3 normal = normalize(fragNormal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0);
	vec3 diffuse = diff * vec3(texture(material.diffuse, texCoord)) * light.diffuse;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
	vec3 specular = spec * vec3(texture(material.specular, texCoord))  * light.specular;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}