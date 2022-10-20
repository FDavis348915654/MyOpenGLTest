#version 330 core
// 实例化

// 分开 Material 和 Light 的定义是为了方便独立控制颜色和亮度

in VS_OUT {
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangetFragPos;
} fs_in;

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

uniform sampler2D texture_diffuse1; // 漫反射的表面颜色
uniform sampler2D texture_specular1; // 反射贴图
uniform sampler2D texture_ambient1; // 反射贴图(用环境光贴图替换)
uniform samplerCube skybox; // 天空盒
uniform sampler2D texture_normal1; // 法线贴图

uniform bool useNormalMap;

void main()
{
	// 从法线贴图范围[0,1]获取法线
	vec3 normal = texture(texture_normal1, fs_in.TexCoords).rgb;
	// 将法线向量转换为范围[-1,1]
	if (useNormalMap) {
		normal = normalize(normal * 2.0 - 1.0);
	}
	else {
		normal = normalize(vec3(0.0, 0.0, 1.0)); // 因为是在切线空间计算法线，所以默认的法线就是这个方向
	}

	vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb;
	vec3 lightColor = vec3(0.2);

	// 平常的光照处理
	// ambient
	vec3 ambient = 0.1 * color;
	// diffuse
	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangetFragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * color;
	// specular
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangetFragPos);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	vec3 specular = spec * lightColor;

	vec3 lighting = (ambient + diffuse + specular);
	FragColor = vec4(lighting, 1.0);
}
