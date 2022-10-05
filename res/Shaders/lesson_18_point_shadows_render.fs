#version 330 core
// 点光源阴影

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float near_plane;
uniform float far_plane;
uniform bool shadows;

float ShadowCalculation(vec3 fragPos) {
	// 从光到片段的向量, 用于采样立方体贴图
	vec3 fragToLight = fragPos - lightPos;
	// [0.0, 1.0]
	float closestDepth = texture(depthMap, fragToLight).r;
	// it is currently in linear range between [0,1], let's re-transform it back to original depth value
	//closestDepth = near_plane * (1.0 - closestDepth) + far_plane * closestDepth;
	closestDepth = closestDepth * (far_plane - near_plane) + near_plane;
	float currentDepth = length(fragToLight);
	float bias = 0.05;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	// display closestDepth as debug (to visualize depth cubemap)
    //FragColor = vec4(vec3((closestDepth - near_plane) / (far_plane - near_plane)), 1.0);
	return shadow;
}

void main()
{
	vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
	vec3 normal = normalize(fs_in.Normal);
	vec3 lightColor = vec3(0.3);
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
	// calculate shadow
	float shadow = shadows ? ShadowCalculation(fs_in.FragPos) : 0.0;
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
	FragColor = vec4(lighting, 1.0);
	//FragColor = vec4(texture(diffuseTexture, fs_in.TexCoords).rgb, 1.0);
}
