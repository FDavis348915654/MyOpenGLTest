#version 330 core
// 视差贴图, 手工计算法线

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

in VS_OUT {
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangetFragPos;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform bool useDepthMap;
uniform bool useNormalMap;

uniform float heightScale;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir) {
	if (useDepthMap) {
		float height = texture(depthMap, texCoords).r;
		return texCoords - viewDir.xy * (height * heightScale);
	}
	else {
		return texCoords;
	}
}

void main()
{
	// 视差贴图的观察方向
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangetFragPos);
	vec2 texCoords = fs_in.TexCoords;

	if (useDepthMap) {
		texCoords = ParallaxMapping(texCoords, viewDir);
		if (texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0) {
			discard;
		}
	}

	// 从法线贴图范围[0,1]获取法线
	vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
	// 将法线向量转换为范围[-1,1]
	if (useNormalMap) {
		normal = normalize(normal * 2.0 - 1.0);
	}
	else {
		normal = normalize(vec3(0.0, 0.0, 1.0));
	}

	vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
	vec3 lightColor = vec3(0.2);

	// 平常的光照处理
	// ambient
	vec3 ambient = 0.1 * color;
	// diffuse
	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangetFragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * color;
	// specular
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	vec3 specular = spec * lightColor;

	// vec3 lighting = (ambient + diffuse + specular) * color;
	vec3 lighting = (ambient + diffuse + specular);
	// vec3 lighting = (diffuse) * color; // 调试, 不带阴影
	FragColor = vec4(lighting, 1.0);
	// FragColor = vec4(texture(diffuseMap, fs_in.TexCoords).rgb, 1.0); // 调试, 不带阴影
}
