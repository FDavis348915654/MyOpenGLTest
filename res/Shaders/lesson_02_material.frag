#version 330 core
// 材质

// 分开 Material 和 Light 的定义是为了方便独立控制颜色和亮度

struct Material {
	vec3 ambient; // 环境光的表面颜色
	vec3 diffuse; // 漫反射的表面颜色
	vec3 specular; // 镜面高光的颜色
	float shininess; // 反光度, 影响镜面高光的散射/半径
};

struct Light {
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;

out vec4 fragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
	// 环境光
	vec3 ambient = light.ambient * material.ambient;
	// 漫反射
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);
	// 镜面反射
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess); // 反光度
	vec3 specular = light.specular * (spec * material.specular);
	
	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result, 1.0f);
}
