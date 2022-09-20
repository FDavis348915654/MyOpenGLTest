#version 330 core
// 投光物 // 聚光 // 软化

// 分开 Material 和 Light 的定义是为了方便独立控制颜色和亮度

struct Material {
	sampler2D diffuse; // 漫反射的表面颜色
	sampler2D specular; // 镜面高光的颜色
	float shininess; // 反光度, 影响镜面高光的散射/半径
};

struct Light {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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
	
	// 聚光的夹角 cos 值
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float insensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	// 将不对环境光做出影响，让它总是能有一点光
	diffuse *= insensity;
	specular *= insensity;
	// 练习题, 尝试实验一下上面的所有光照类型和它们的片段着色器。试着对一些向量进行取反，并使用 < 来代替 >。试着解释不同视觉效果产生的原因。
	//diffuse *= (1.0 - insensity);
	//specular *= (1.0 - insensity);
	
	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result, 1.0f);
}
