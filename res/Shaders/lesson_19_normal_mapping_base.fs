#version 330 core
// 法线贴图

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D normalMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
	//vec3 normal = normalize(fs_in.Normal);
	// 从法线贴图范围[0,1]获取法线
	vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
	// 将法线向量转换为范围[-1,1]
	normal = normalize(normal * 2.0 - 1.0)
	vec3 lightColor = vec3(0.3);
	// 平常的光照处理
	// ambient
	vec3 ambient = 0.3 * color;
	// diffuse
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;
	// specular
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;
	vec3 lighting = (ambient + diffuse + specular) * color;
	//vec3 lighting = (diffuse) * color; // 调试, 不带阴影
	FragColor = vec4(lighting, 1.0);
	//FragColor = vec4(texture(diffuseTexture, fs_in.TexCoords).rgb, 1.0); // 调试, 不带阴影
}
