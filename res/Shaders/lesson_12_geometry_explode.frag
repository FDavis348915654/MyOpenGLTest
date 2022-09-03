#version 330 core
// 几何着色器

// 分开 Material 和 Light 的定义是为了方便独立控制颜色和亮度

in vec2 TexCoords;

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

uniform sampler2D texture_diffuse1; // 漫反射的表面颜色
uniform sampler2D texture_specular1; // 反射贴图
uniform sampler2D texture_ambient1; // 反射贴图(用环境光贴图替换)
uniform samplerCube skybox; // 天空盒
uniform vec3 cameraPos;

void main()
{
	vec3 result = texture(texture_diffuse1, TexCoords).rgb;
	FragColor = vec4(result, 1.0);
}
