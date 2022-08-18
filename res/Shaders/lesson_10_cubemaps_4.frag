#version 330 core
// 立方体贴图 模型

// 分开 Material 和 Light 的定义是为了方便独立控制颜色和亮度

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

uniform sampler2D texture_diffuse1; // 漫反射的表面颜色
uniform sampler2D texture_specular1; // 反射贴图
uniform samplerCube skybox; // 天空盒
uniform vec3 cameraPos;

void main()
{
	vec3 I = normalize(FragPos - cameraPos);
	vec3 R = reflect(I, normalize(Normal));
	vec3 skyColor = texture(skybox, R).rgb * texture(texture_specular1, TexCoords).rgb;
	vec3 result = vec3(texture(texture_diffuse1, TexCoords)) + skyColor;
	//vec3 result = vec3(texture(texture_diffuse1, TexCoords));
	FragColor = vec4(result, 1.0);
}
