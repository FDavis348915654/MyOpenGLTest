#version 330 core
// 模型

// 分开 Material 和 Light 的定义是为了方便独立控制颜色和亮度

struct DirLight {
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

struct PointLight {
	vec3 position;
	
	float constant;
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

#define NR_POINT_LIGHTS 4

uniform vec3 viewPos;
// Material
uniform sampler2D texture_diffuse1; // 漫反射的表面颜色
uniform sampler2D texture_specular1; // 镜面高光的颜色
uniform float shininess; // 反光度, 影响镜面高光的散射/半径

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform bool useSpotLight;

void main()
{
	// 属性
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	// 第一阶段：定向光照
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	// 第二阶段：点光源
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}
	// 第三阶段：聚光
	if (useSpotLight) {
		result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
	}
	FragColor = vec4(result, 1.0);
}

// 定向光照
// 注意, 这里的 viewDir 仅仅是片段看向 viewPos 的方向
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	// 漫反射着色
	float diff = max(dot(normal, lightDir), 0.0);
	// 镜面光着色
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	// 合并结果
	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
	return (ambient + diffuse + specular);
}

// 点光源
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	// 漫反射着色
	float diff = max(dot(normal, lightDir), 0.0);
	// 镜面光着色
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	// 衰减
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// 合并结果
	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

// 聚光
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	// 漫反射着色
	float diff = max(dot(normal, lightDir), 0.0);
	// 镜面光着色
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	// 聚光着色
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float insensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	// 合并结果
	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
	diffuse *= insensity;
	specular *= insensity;
	return (ambient + diffuse + specular);
}
