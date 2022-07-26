#version 330 core
// 投光物 // 点光源

// 分开 Material 和 Light 的定义是为了方便独立控制颜色和亮度

struct Material {
	sampler2D diffuse; // 漫反射的表面颜色
	sampler2D specular; // 镜面高光的颜色
	float shininess; // 反光度, 影响镜面高光的散射/半径
};

struct Light {
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 fragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
	// 点光源衰减
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	// 环境光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	// 漫反射
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	// 镜面反射
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess); // 反光度
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	// 各种光的类型都衰减
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result, 1.0f);
}
