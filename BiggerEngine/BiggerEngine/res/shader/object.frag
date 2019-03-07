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

struct DirectLight{
	//vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform DirectLight directLight;

struct PointLight{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
    float linear;
    float quadratic;
};

uniform PointLight pointLight;

struct SpotLight{
	vec3  position;
    vec3  direction;
    float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;


};

uniform SpotLight spotLight;


uniform vec3 viewPos;

// Direct light
vec3 caculateDirectLight(DirectLight directLight, vec3 objectDiffuse, vec3 objectSpecular, vec3 normal, vec3 viewDir){
	
	vec3 directAmbient = directLight.ambient * objectDiffuse;

	vec3 directLightDir = normalize(-directLight.direction);
	float directDiff = max(dot(normal, directLightDir), 0);
	vec3 directDiffuse = directDiff * directLight.diffuse  * objectDiffuse ;

	vec3 directReflectDir = reflect(-directLightDir, normal);
	float directSpec = pow(max(dot(viewDir, directReflectDir), 0), material.shininess);
	vec3 directSpecular = directSpec * directLight.specular *  objectSpecular;

	return directAmbient + directDiffuse + directSpecular;
}

// Point light
vec3 caculatePointLight(PointLight pointLight, vec3 objectDiffuse, vec3 objectSpecular, vec3 normal, vec3 viewDir){
	float distance = length(pointLight.position - fragPos);
	float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));
	vec3 pointAmbient = pointLight.ambient * attenuation * objectDiffuse;

	vec3 pointLightDir = normalize(pointLight.position - fragPos);
	float pointDiff = max(dot(normal, pointLightDir), 0);
	vec3 pointDiffuse = pointDiff * pointLight.diffuse * attenuation * objectDiffuse;

	vec3 pointReflectDir = reflect(-pointLightDir, normal);
	float pointSpec = pow(max(dot(viewDir, pointReflectDir), 0), material.shininess);
	vec3 pointSpecular = pointSpec * pointLight.specular * attenuation *  objectSpecular;

	return pointAmbient + pointDiffuse + pointSpecular;
}

vec3 caculateSpotLight(SpotLight spotLight, vec3 objectDiffuse, vec3 objectSpecular, vec3 normal, vec3 viewDir){
	vec3 spotLightDir = normalize(spotLight.position - fragPos);
	float theta  = dot(spotLightDir, normalize(-spotLight.direction));
	float epsilon   = spotLight.cutOff -  spotLight.outerCutOff;
	float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0); 

	vec3 spotAmbient = spotLight.ambient * objectDiffuse;

	vec3 spotDiffuse = max(dot(normal, spotLightDir), 0) * spotLight.diffuse * objectDiffuse * intensity;

	vec3 spotLightReflectDir = reflect(-spotLightDir, normal);
	float spotSpec = pow(max(dot(viewDir, spotLightReflectDir), 0), material.shininess);
	vec3 spotSpecular = spotSpec * spotLight.specular * objectSpecular * intensity;
	return spotAmbient + spotDiffuse + spotSpecular;
}

void main()
{

	vec3 objectDiffuse = vec3(texture(material.diffuse, texCoord));
	vec3 objectSpecular = vec3(texture(material.specular, texCoord));
	vec3 normal = normalize(fragNormal);
	vec3 viewDir = normalize(viewPos - fragPos);

	

	vec3 directLightColor = caculateDirectLight(directLight, objectDiffuse, objectSpecular, normal, viewDir);
	
	vec3 pointLightColor = caculatePointLight(pointLight, objectDiffuse, objectSpecular, normal, viewDir);

	vec3 spotLightColor = caculateSpotLight(spotLight, objectDiffuse, objectSpecular, normal, viewDir);
	


    FragColor = vec4(directLightColor + pointLightColor + spotLightColor, 1.0);
}