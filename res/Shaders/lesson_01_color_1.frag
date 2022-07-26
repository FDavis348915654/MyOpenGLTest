#version 330 core

// 基础光照 // 练习题三, 在观察空间（而不是世界空间）中计算冯氏光照

in vec3 Normal;
in vec3 FragPos;
in vec3 VlightPos;

out vec4 fragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	// 环境光
	float ambientStrength = 0.1f; // 环境光强度
	vec3 ambient = ambientStrength * lightColor;
	// 漫反射
	float diffStrength = 1.0f;
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(VlightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0) * diffStrength;
	vec3 diffuse = diff * lightColor;
	// 镜面反射
	float specularStrength = 0.5f; // 镜面强度
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32); // 反光度
	vec3 specular = specularStrength * spec * lightColor;
	
	vec3 result = (ambient + diffuse + specular) * objectColor;
	fragColor = vec4(result, 1.0f);
}
